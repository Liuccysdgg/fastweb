#pragma once
#include "net/http_request.h"
#include "net/http_response.h"
#include "sol/sol.hpp"

namespace module
{
    class response
    {
    public:
        response(network::http::response* response);
        ~response();

        bool send_data(const char* buf, size_t buf_len, ushort stateNum, const std::string& stateDesc);
        bool send(const std::string& value);
        bool sendex(const std::string& value, ushort stateNum, const std::string& stateDesc);
        bool send_file(const std::string& filepath, int32 downbaud, ushort stateNum, const std::string& stateDesc);
        bool redirect(const std::string& filepath, bool MovedPermanently = false);
        bool forward(const std::string& filepath);
        void header(const std::string& name, const std::string& value);

        void set(const std::string& name,const std::string& value);
        void sets(const sol::table& lua_table);
        static void regist(sol::state* lua);
    private:
        network::http::response* m_response = nullptr;
        std::map<std::string, std::string> m_sets;
    };
}

