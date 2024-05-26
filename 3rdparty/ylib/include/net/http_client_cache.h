#pragma once
#include "http_define.h"
#if USE_NET_HTTP_CLIENT
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class client_plus;
            //客户端缓存
            class client_cache
            {
            public:
                client_cache();
                ~client_cache();
                void open(const std::string& dirpath);
                bool read(network::http::client_plus* client, ylib::buffer& cache);
                void set_header(network::http::client_plus* client, const std::string& url);
                void close();
                void write(network::http::client_plus* client);
                bool read(const std::string& url, ylib::buffer& cache);
            private:
                bool m_open;
                std::string m_dirpath;
            };
        }
    }
}
#endif
