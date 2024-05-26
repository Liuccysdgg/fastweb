#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "http_server.h"
#include "util/strutils.h"
#include "util/time.h"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class server;
            class response;
            class request;
            class website;
            /*************************************************************************
             * class：请求包
             *************************************************************************/
            class reqpack
            {
            public:

                reqpack();
                ~reqpack();

                void init(const std::string& url, const std::string& host, const ylib::buffer& data, uint64 connid, network::http::server* server);
                void clear();
                network::http::request* request();
                network::http::response* response();

                const std::string& host()
                {
                    /*获取基本请求信息*/
                    return m_host;
                }
                network::http::method method();
                const std::string& filepath();
                void filepath(const std::string& path);
                network::http::server* server()
                {
                    return m_server;
                }
                ylib::buffer& data()
                {
                    return m_data;
                }
                const std::string& url()
                {
                    return m_url;
                }
                const std::string& referrer()
                {
                    return m_referrer;
                }
                const uint64& connid()
                {
                    return m_connid;
                }
                network::http::website* website() {
                    return m_website;
                }
                void website(network::http::website* website) {
                    m_website = website;
                }
                timestamp begin_msec() {
                    return m_begin_msec;
                }
                std::string exec_msec() {
                    return std::to_string(time::now_msec() - m_begin_msec);
                }
                const std::string& remote() {
                    if (m_remote_ipaddress.empty()) {
                        ushort port;
                        m_server->remote(connid(), m_remote_ipaddress, port);
                    }
                    return m_remote_ipaddress;
                }
                ylib::json& extra() { return m_extra; }
            private:
                // 请求主机
                std::string m_host;
                // 请求方式
                network::http::method m_method;
                // 请求路径
                std::string m_filepath;
                // HPSERVER
                network::http::server* m_server;
                // 转发来路
                std::string m_referrer;
                // 请求ID
                uint64 m_connid;
                // 站点
                network::http::website* m_website;
                // 接收数据
                ylib::buffer m_data;
                // URL
                std::string m_url;
                // 请求发起时间
                timestamp m_begin_msec;
                // 远程IP
                std::string m_remote_ipaddress;

                // 附加数据
                ylib::json m_extra;
            private:
                network::http::request* m_request;
                network::http::response* m_response;
            };
        }
    }
}
#endif
