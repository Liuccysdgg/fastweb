#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "http_interface.h"
#include <functional>
#include <map>
#include <regex>
#include "util/array.hpp"
#include "util/queue.hpp"
#include "util/thread.h"
//#include "sol/sol.hpp"
class IHPThreadPool;
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class reqpack;
            class http_server_lst;
            class request;
            class response;
            class interceptor;
            /*************************************
             * struct：订阅信息
             *************************************/
            struct subscribe_info {
                subscribe_info()
                {
                    method = network::http::ALL;
                    controller = false;
                    controller_function = nullptr;
                }
                // 请求路径
                std::regex express;
                // 请求类型
                network::http::method method;
                // 回调函数
                std::function<void(network::http::request*, network::http::response*)> callback;
                // 控制器。为控制器则启动下面两个属性
                bool controller;
                // 创建控制器类指针
                std::function<void* ()> create_controller_callback;
                // 控制器函数
                HTTP_CTR_FUNCTION controller_function;
#if HTTP_LUA_ENGINE == 1
                // LUA
                std::string lua_filepath;
                // LUA虚拟机初始化回调
                std::function<void(sol::state& state)> lua_init_state;
#endif
            };
            /*************************************************************************
             * class：路由中专服务
             *************************************************************************/
            class router :public ylib::error_base, public network::http::interface_, private ylib::ithread
            {
            public:
                /*线程参数信息*/
                struct thread_param_info
                {
                    void clear() {}
                    network::http::router* router;
                    network::http::reqpack* reqpack;
                };

            public:
                router();
                ~router();
                /******************************************************************
                 * function：启动
                 * param
                 *      config                  ：           配置项
                 * return：
                 *      失败可通过 last_error() 返回错误信息。
                 ******************************************************************/
                bool start(const router_config& config);
                /******************************************************************
                 * function：关闭
                 ******************************************************************/
                void close();
                /******************************************************************
                 * function：拦截器
                 ******************************************************************/
                network::http::interceptor* interceptor();
                /******************************************************************
                 * function：订阅
                 * desc：浏览器请求会首先触发订阅，订阅未找到符合项则触发 other 传入函数。
                 * param
                 *      path                    ：           路径
                 *      method                  ：           请求类型
                 *      callback                ：           触发回调
                 * return：
                 *      同一地址不允许订阅两次
                 ******************************************************************/
                void subscribe(
                    const std::string& path,
                    network::http::method method,
                    std::function<void(network::http::request*, network::http::response*)> callback
                );
#define SUBSCRIBE(ROUTER,CONTROLLER,FUNCTION,PATH,METHOD) ROUTER->subscribe([]()->void*{return new CONTROLLER;},(ylib::network::http::HTTP_CTR_FUNCTION)&CONTROLLER::FUNCTION,PATH,METHOD)

                void subscribe(
                    std::function<void* ()> create_controller_callback,
                    network::http::HTTP_CTR_FUNCTION function,
                    std::string path,
                    network::http::method method
                ); 
#if HTTP_LUA_ENGINE == 1
                void subscribe(
                    const std::string& lua_filepath,
                    std::string path,
                    network::http::method method,
                    std::function<void(sol::state& state)> init_state = nullptr
                );
#endif
                /******************************************************************
                 * function：其它
                 * desc：未订阅请求触发该回调
                 * param
                 *      callback                ：           触发回调
                 ******************************************************************/
                void other(std::function<void(network::http::request*, network::http::response*)> callback);
                /******************************************************************
                 * function：数据接收后回调
                 ******************************************************************/
                void on_recved(std::function<void(const ylib::buffer& begin, ylib::buffer* end)> callback);
                /******************************************************************
                 * function：数据发送前回调
                 * desc：不支持大文件断点传输方式
                 ******************************************************************/
                void on_sendbefore(std::function<void(const ylib::buffer& begin, ylib::buffer* end)> callback);
                /******************************************************************
                * function：线程回调[禁止调用]
                * desc：put提交后投递到线程池，线程池开始执行调用该回调。
                * param
                *      param                    ：           线程参数
                ******************************************************************/
                void __thread_callback(reqpack* rq);
                friend class http_server_lst;
                friend class response;
                /******************************************************************
                 * function：队列数
                 ******************************************************************/
                size_t queue_size();
            private:
                // 添加任务
                void push(reqpack* rp);
                // 是否为代理任务
                bool is_proxy(reqpack* rp);
                // 是否为CDN服务
                bool is_cdn(reqpack* rp);
#if HTTP_LUA_ENGINE == 1
                // LUA执行
                void lua_engine(reqpack *rp,const network::http::subscribe_info& info);
#endif
            private:
                // 订阅列表
                ylib::nolock_array<network::http::subscribe_info*> m_subscribe;
                // 线程池
                IHPThreadPool* m_threadpool;
                // [回调] 未订阅请求
                std::function<void(network::http::request*, network::http::response*)> m_callback_other;
                // [回调] 接收后
                std::function<void(const ylib::buffer& begin, ylib::buffer* end)> m_callback_recved;
                // [回调] 发送前
                std::function<void(const ylib::buffer& begin, ylib::buffer* end)> m_callback_sendbefore;
                // 拦截器
                network::http::interceptor* m_interceptor;
            private:
                ylib::queue<network::http::reqpack*> m_handle_queue;
                router_config m_config;
                // 通过 ithread 继承
                virtual bool run() override;
            };
        }
    }
}
#endif
