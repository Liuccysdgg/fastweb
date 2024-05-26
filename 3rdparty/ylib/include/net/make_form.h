#pragma once
#include "http_define.h"

#if USE_NET_HTTP_UTIL
#include "util/vector.hpp"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class make_form
            {
            public:
                make_form();
                ~make_form();
                bool add(const std::string& name, const std::string& value);
                bool add(const std::string& name, const std::string& filename, const std::string& content_type, const ylib::buffer& data);
                bool make(ylib::buffer& data, std::string& boundary) const;
            private:
                ylib::vector<form_info*> m_list;
                ylib::buffer m_data;
            };
        }
    }
}
#endif
