#pragma once
#include "http_define.h"
#if USE_NET_HTTP_CLIENT
#include <functional>
#include "http_header.h"
#include "http_client_cache.h"
#include "http_cookie.h"
#include "make_form.h"

class http_client_listener;
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class client_plus :public ylib::error_base
            {
            public:
                client_plus(); 
                ~client_plus();
                void close();
                void set_timeout(uint32 connect_msec = 3000, uint32 recv_msec = 8000);
                bool del(const std::string& url, const std::map<std::string, std::string>& value = std::map<std::string, std::string>());
                bool get(const std::string& url, const std::map<std::string, std::string>& value = std::map<std::string, std::string>(), bool wait = true);
                bool post(const std::string& url, const std::map<std::string, std::string>& value, bool to_utf8 = false);
                bool post(const std::string& url, const ylib::json& value, bool to_utf8 = false);
                bool post(const std::string& url, const ylib::buffer& value);
                bool post(const std::string& url, const http::make_form& value);
                bool head(const std::string& url);
                /// <summary>
                /// 连接代理服务器
                /// </summary>
                /// <param name="address"></param>
                /// <param name="port"></param>
                /// <returns></returns>
                void setproxy(const std::string& address,ushort port);
                network::http::header_list& headers_request();
                network::http::header_list& headers_response();
                uint32 status();
                ylib::buffer& response();
                /*设置缓存*/
                void cache(client_cache* cache);
                /*请求路径*/
                inline const std::string& url() { return m_url; }
                /*cookie*/
                inline network::http::cookie& cookie() { return m_cookie; }
                inline void cookie(const network::http::cookie& ck) { m_cookie = ck; }

                /*[回调] 正在下载*/
                void on_down_ing(const std::function<bool(void* data, uint32 downsize, uint64 alldownsize, uint64 allsize, network::http::client_plus& client)>& callback);
                /*[回调] 下载结束*/
                void on_down_end(const std::function<void(network::http::client_plus& client)>& callback);
                /*[回调] 下载失败*/
                void on_down_failed(const std::function<void(network::http::client_plus& client)>& callback);

                // 清理所有Cookie缓存
                static void clear_all_cookies();

                friend class http_client_listener;
            public:
                uint64 m_temp[10];
            private:
                bool parseurl(std::string url);
                bool connect();
                bool init();
                bool request();
                bool post(const std::string& url);
                void* client();


                bool init_proxy();
            private:
                // HP客户端
                void* m_client;
                // HP客户端-SSL
                void* m_client_ssl;
                // HP监听器
                http_client_listener* m_listener;
                // 是否已初始化
                bool m_init;
                // 连接IP
                std::string m_ipaddress;
                // 连接端口
                ushort m_port;
                // HTTPS
                bool m_ssl;
                // 请求链接
                std::string m_url;
                // 请求路径
                std::string m_path;
                // 请求方式
                network::http::method m_method;
                // 请求数据
                ylib::buffer m_request_body;
                // [header] 请求
                network::http::header_list m_headers_request;
                // 超时时间 连接
                timestamp m_timeout_connect_msec;
                // 超时时间 接收
                timestamp m_timeout_recv_msec;
                // 缓存
                client_cache* m_cache;
                // cookie
                network::http::cookie m_cookie;
                // 请求等待
                bool m_request_wait = true;
#ifndef _WIN32
            public:
#endif
                // 关闭
                bool m_close;
                // 代理服务器
                ylib::AddressPort m_proxy;
            };
        }
    }
}
#endif
