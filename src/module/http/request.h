#pragma once
#include "net/http_request.h"
#include "net/http_response.h"

#include "sol/sol.hpp"
#include "core/define.h"
namespace module
{
    class session;
    class request
    {
    public:
        request(network::http::request* request);
        ~request();
        /// <summary>
        /// 取协议头
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        std::string header(const std::string& name);
        /// <summary>
        /// 取请求类型
        /// </summary>
        /// <returns></returns>
        std::string method();
        /// <summary>
        /// 取请求路径
        /// </summary>
        /// <returns></returns>
        std::string filepath();
        /// <summary>
        /// 取Host主机
        /// </summary>
        /// <returns></returns>
        std::string host();
        /// <summary>
        /// 取请求参数
        /// </summary>
        /// <param name="name"></param>
        /// <param name="throw_"></param>
        /// <param name="s"></param>
        /// <returns></returns>
        sol::object param(const std::string& name,bool throw_,sol::this_state s);
        /// <summary>
        /// 取远程地址
        /// </summary>
        /// <returns></returns>
        std::string remote_ipaddress();
        /// <summary>
        /// 取远程端口
        /// </summary>
        /// <returns></returns>
        ushort remote_port();
        /// <summary>
        /// 取session
        /// </summary>
        /// <param name="token"></param>
        /// <returns></returns>
        module::session* session(const std::string& token);
        /// <summary>
        /// 取body参数
        /// </summary>
        /// <param name="s"></param>
        /// <returns></returns>
        sol::table body_param(sol::this_state s);
        /// <summary>
        /// 取URL参数
        /// </summary>
        /// <param name="s"></param>
        /// <returns></returns>
        sol::table url_param(sol::this_state s);
        /// <summary>
        /// 取请求体
        /// </summary>
        /// <returns></returns>
        std::string body();
        /// <summary>
        /// 取表单列表
        /// </summary>
        /// <returns></returns>
        sol::table multipart(sol::this_state s);
        /// <summary>
        /// 取表单内容文本
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        std::string multipart_content(int id);
        /// <summary>
        /// 保存文件
        /// </summary>
        /// <param name="id"></param>
        /// <param name="filepath"></param>
        /// <returns></returns>
        bool multipart_content_save(int id,const std::string& filepath);


        /// <summary>
        /// 置临时数据
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        void set(const std::string& name,const std::string& value);
        /// <summary>
        /// 取临时数据
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        std::string get(const std::string& name);
        /// <summary>
        /// 网站指针
        /// </summary>
        /// <returns></returns>
        network::http::website* website();
        /// <summary>
        /// 保存请求体
        /// </summary>
        /// <returns></returns>
        bool save_body(const std::string& filepath);
        static void regist(sol::state* lua);
    private:
        void multipart_content_check(int id);

        bool request_param(const std::string& name, std::string& value);
    private:
        network::http::request* m_request = nullptr;
        module::session* m_session = nullptr;

    };

}
