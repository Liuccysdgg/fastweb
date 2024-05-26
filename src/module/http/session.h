#pragma once
#include "net/http_request.h"
#include "net/http_session.h"
#include "sol/sol.hpp"
#include "request.h"
namespace module
{
    class session
    {
    public:
        session(network::http::session* session);
        ~session();
        void init(module::request& request, const std::string& id);
        std::string id();
        void update();
        void set(const std::string& name, const std::string& value);
        std::string get(const std::string& name);
        bool check();
        static void regist(sol::state& state);
    private:
        network::http::session* m_session = nullptr;
    };
}

