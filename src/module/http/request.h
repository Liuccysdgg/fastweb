#pragma once
#include "net/http_request.h"
#include "net/http_response.h"

#include "sol/sol.hpp"
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
        std::string pstring(const std::string& name);
        int64 pinteger(const std::string& name);
        double pnumber(const std::string& name);
        std::string remote_ipaddress(bool find_header = false, const std::string& inside_ipaddress = "");
        ushort remote_port();

        std::string token();
        module::session* session(const std::string& token);

        void* website();

        static void regist(sol::state& state);
    private:
        bool request_param(const std::string& name, std::string& value);
    private:
        network::http::request* m_request = nullptr;
        module::session* m_session = nullptr;
    };

}
