#pragma once
#include "http_define.h"
#include "http_reqpack.h"
#include "http_interface.h"
#include "util/map.hpp"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class cache :public network::http::interface_, public ylib::error_base
            {
            public:
                cache();
                ~cache();
                bool start(const server_cache_config& config);
                void stop();
                bool have_send(network::http::reqpack* rp);
                bool enable();
                std::string make_key(const std::string& filepath);

                bool find_ext(const std::string& filepath);

                std::string make_cache_filepath(const std::string& filepath);
            private:
                server_cache_config m_config;

                ylib::map<std::string, std::string> m_keys;
            };
        }
    }
}
