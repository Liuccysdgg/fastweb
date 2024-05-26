#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <functional>
#include <vector>
#include "util/map.hpp"
#include "util/json.h"


namespace ylib
{
    namespace network
    {
        namespace http
        {
            /*绑定域名*/
            struct domain_info
            {
                domain_info() {
                    https = false;
                }
                // 是否HTTPS
                bool https;
                // 域名
                std::string domain;
            };

            class server;
            class router;
            class website;

            /***************************************************************
             * Class：控制中心
             ***************************************************************/
            class center :public ylib::error_base
            {
            public:

                center();
                ~center();
                /******************************************************************
                 * function：创建
                 * param
                 *     config                  ：           配置项
                 * return：
                 *      失败可通过 last_error() 返回错误信息。
                 ******************************************************************/
                bool create(const start_config& config);

                bool start();

                /******************************************************************
                 * function：关闭
                 ******************************************************************/
                void close();

                network::http::server* server(ushort port);

                network::http::website* website(const std::string& host);
                network::http::website* website_byname(const std::string& name);


                const start_config& config() { return m_config; }
            private:
                //所有监听端口
                std::vector<ushort> listen_ports();
                //端口是否需要SSL
                bool port_have_ssl(ushort port);
            private:
                // HTTP 服务
                std::vector<network::http::server*> m_server;
                // 站点
                std::vector<network::http::website*> m_website;
                // 配置缓存
                //ylib::map<std::string, std::string> m_config;
                // 启动信息
                start_config m_config;
            public:
                uint64 m_temp[10];

            };
        }
    }
}

#endif
