#pragma once
#include "net/http_request.h"
#include "net/http_response.h"

#include "sol/sol.hpp"
#include "core/define.h"
namespace module
{
    class session;
    class request
    {
    public:
        request(network::http::request* request);
        ~request();

        std::string header(const std::string& name);
        network::http::method method();
        std::string filepath();
        std::string host();
        VarType param(const std::string& name,bool throw_,sol::this_state s);
        std::string remote_ipaddress();
        ushort remote_port();

        std::string token();
        module::session* session(const std::string& token);

        sol::table body_param(sol::this_state s);
        sol::table url_param(sol::this_state s);

        std::string body();
        void* website();

        static void regist(sol::state* lua);
    private:
        bool request_param(const std::string& name, std::string& value);
    private:
        network::http::request* m_request = nullptr;
        module::session* m_session = nullptr;
    };

}
