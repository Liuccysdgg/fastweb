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
        /// <summary>
        /// 取协议头
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        std::string header(const std::string& name);
        /// <summary>
        /// 取请求类型
        /// </summary>
        /// <returns></returns>
        network::http::method method();
        /// <summary>
        /// 取请求路径
        /// </summary>
        /// <returns></returns>
        std::string filepath();
        /// <summary>
        /// 取Host主机
        /// </summary>
        /// <returns></returns>
        std::string host();
        /// <summary>
        /// 取请求参数
        /// </summary>
        /// <param name="name"></param>
        /// <param name="throw_"></param>
        /// <param name="s"></param>
        /// <returns></returns>
        sol::object param(const std::string& name,bool throw_,sol::this_state s);
        /// <summary>
        /// 取远程地址
        /// </summary>
        /// <returns></returns>
        std::string remote_ipaddress();
        /// <summary>
        /// 取远程端口
        /// </summary>
        /// <returns></returns>
        ushort remote_port();
        /// <summary>
        /// 取token
        /// </summary>
        /// <returns></returns>
        std::string token();
        /// <summary>
        /// 取session
        /// </summary>
        /// <param name="token"></param>
        /// <returns></returns>
        module::session* session(const std::string& token);
        /// <summary>
        /// 取body参数
        /// </summary>
        /// <param name="s"></param>
        /// <returns></returns>
        sol::table body_param(sol::this_state s);
        /// <summary>
        /// 取URL参数
        /// </summary>
        /// <param name="s"></param>
        /// <returns></returns>
        sol::table url_param(sol::this_state s);
        /// <summary>
        /// 取请求体
        /// </summary>
        /// <returns></returns>
        std::string body();
        /// <summary>
        /// 取网站指针
        /// </summary>
        /// <returns></returns>
        void* website();

        static void regist(sol::state* lua);
    private:
        bool request_param(const std::string& name, std::string& value);
    private:
        network::http::request* m_request = nullptr;
        module::session* m_session = nullptr;
    };

}
