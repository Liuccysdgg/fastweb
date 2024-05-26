#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <string>
#include <vector>
#include <list>

#include "http_header.h"
#include "http_interface.h"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class server;
            class reqpack;
            class response :public network::http::interface_
            {
            public:
                response();
                ~response();
                void init(reqpack* rp);
                bool send(const char* buf, size_t buf_len, ushort stateNum = 200, const std::string& stateDesc = "OK");
                bool send(const ylib::buffer& value, ushort stateNum = 200, const std::string& stateDesc = "OK");
                bool send(const std::string& value, ushort stateNum = 200, const std::string& stateDesc = "OK");
                bool send(const ylib::json& json, ushort stateNum = 200, const std::string& stateDesc = "OK");
                bool send_file(const std::string& filepath, int32 downbaud = -1, ushort stateNum = 200, const std::string& stateDesc = "OK");
                std::map<std::string, std::string>* headers(); 
                bool redirect(const std::string& filepath, bool MovedPermanently = false);
                bool forward(const std::string& filepath);
            public:
                ylib::json sjson;
            private:
                bool filecache(const uint64& last_modify_time);
                bool fileoffset(long filesize, long& start, long& len);
            private:
                std::map<std::string, std::string> m_headers;
                bool m_response;
                network::http::reqpack* m_reqpack;

            };
        }
    }
}

#endif
