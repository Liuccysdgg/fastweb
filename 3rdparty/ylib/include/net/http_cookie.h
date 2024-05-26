#pragma once
#include "http_define.h"
#if USE_NET_HTTP_UTIL
#include <map>
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class cookie
            {
            public:
                cookie();
                ~cookie();
                void merge(const cookie& ck);
                void merge(const std::string& ck);
                std::string to_string();
                void clear();
            private:
                std::map<std::string, std::string> m_param;
            };
        }
    }
   
}
#endif
