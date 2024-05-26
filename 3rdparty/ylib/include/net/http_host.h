#pragma once
#include "define.h"
#if USE_NET_HTTP_WEBSITE
#include "net/http_interface.h"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            /******************************************************
             * class：域名器
             ******************************************************/
            class ssl;
            class host :public ylib::error_base, public network::http::interface_
            {
            public:
                host();
                ~host();
                void init(const std::string& host, ushort port, network::http::ssl* ssl = nullptr);
                inline network::http::ssl* ssl() { return m_ssl; }
                inline bool equal(const std::string& host) { return m_host == host; }
                inline const std::string& hostname() { return m_return_host; }
            private:
                std::string m_host;
                std::string m_return_host;
                ushort m_port;
                network::http::ssl* m_ssl;
            };
        }
    }
}

#endif
