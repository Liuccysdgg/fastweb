#pragma once
#include "http_define.h"
#include "util/cache.h"
#if USE_NET_HTTP_WEBSITE
class http_agent_listener;
class http_agent_extra;
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class reqpack;
            class server;
            class agent :public ylib::error_base
            {
            public:
                agent();
                ~agent();
                bool start();
                void stop();
                void disconnect(bool ssl, uint64 connid);
                bool request(int32 wait_msec, reqpack* rp, network::http::proxy* proxy);
            private:
                void* get();
            private:
                void* m_agent_ssl;
                void* m_agent;
                http_agent_listener* m_listener;
                bool m_ssl;
            };
        }
    }
    
}
#endif
