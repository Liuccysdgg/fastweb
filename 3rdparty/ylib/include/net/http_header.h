#pragma once
#include "define.h"
#if USE_NET_HTTP_UTIL
#include <map>
namespace ylib
{
    namespace network
    {
        namespace http
        {

            class header
            {
            public:
                header();
                ~header();
                void clear();
                void name(const std::string& value);
                std::string name() const;

                std::map<std::string, std::string> param();

                void parse(const std::string& body);
                void push(const std::string& value);
                void push(const std::string& key, const std::string& value);

                bool exist(const std::string& key);

                std::string to_string() const;
            private:
                std::string m_name;
                std::map<std::string, std::string> m_param;
                std::vector<std::string> m_values;
            };
            class header_list
            {
            public:
                header_list();
                ~header_list();
                bool exist(const std::string& name) const;
                ylib::network::http::header get(const std::string& name);
                void set(const std::string& name, const ylib::network::http::header& header);
                void set(const std::string& name, const std::string& value);
                void del(const std::string& name);
                std::map<std::string, std::string> to() const;
                size_t size();
                void clear();
            private:
                std::map<std::string, header> m_headers;
            };
        }
    }
}
#endif
