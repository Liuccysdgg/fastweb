#pragma once
#include "sol/sol.hpp"
#include "net/http_client_plus.h"
namespace module
{
    class httpclient
    {
    public:
        httpclient();
        ~httpclient();

        void set_timeout(uint32 connect_msec = 3000, uint32 recv_msec = 8000);
        bool get(const std::string& url);
        bool post(const std::string& url,const std::string& body);
        void request_header(const std::string& name,const std::string& value);
        std::string response_header(const std::string& name);
        std::string response();
        ushort status();

        static void regist(sol::state* lua);
    private:
        network::http::client_plus m_client;
    };
}

