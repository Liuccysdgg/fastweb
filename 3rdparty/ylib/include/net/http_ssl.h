#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <functional>
#include "http_interface.h"
#include <map>
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class server;
            /******************************************************
             * class：域名器
             ******************************************************/
            class ssl :public ylib::error_base, public network::http::interface_
            {
            public:
                /**********************************************************************************************
                 * function：构造函数
                 * param
                 *          server                                      ：                                   HTTP服务
                 *          config                                     ：                                   配置
                 **********************************************************************************************/
                ssl(network::http::server* server,ssl_config config);
                ~ssl();
                /**********************************************************************************************
                 * function：注册证书
                 **********************************************************************************************/
                bool regist();
                /**********************************************************************************************
                 * function：绑定
                 **********************************************************************************************/
                bool bind(const std::string& host);
            private:
                ssl_config m_config;
                std::string m_pem_cert_data;
                std::string m_pem_key_data;
                std::string m_pem_ca_data;
                network::http::server* m_server;
                int32 m_index;
            };
        }
    }
}
#endif
