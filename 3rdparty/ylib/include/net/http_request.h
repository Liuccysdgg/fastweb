#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <string>
#include <vector>
#include "http_session.h"
#include "http_parser.h"
#include "http_interface.h"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class reqpack;
            class server;
            /********************************************************************
             * class：Http请求解析类
             ********************************************************************/
            class request :public network::http::interface_
            {
            public: 
                request();
                ~request(); 

                /***************************************************************************
                 * function：取协议头
                 * param
                 *		name                        ：               名称
                 *      value                       ：               内容
                 ***************************************************************************/
                bool header(const std::string& name, std::string& value);

                /***************************************************************************
                 * function：取请求类型
                 ***************************************************************************/
                network::http::method method();
                /***************************************************************************
                 * function：取请求路径
                 ***************************************************************************/
                std::string filepath();
                /***************************************************************************
                 * function：取请求主机
                 ***************************************************************************/
                std::string host();
                /***************************************************************************
                 * function：取Session
                 ***************************************************************************/
                network::http::session& session(const std::string& session_id);
                /// <summary>
                /// 取TOKEN
                /// </summary>
                /// <returns></returns>
                std::string token();


                /***************************************************************************
                 * function：取reqpack
                 ***************************************************************************/
                network::http::reqpack* reqpack();
                /***************************************************************************
                 * function：解析器
                 ***************************************************************************/
                network::http::parser* parser();
                /***************************************************************************
                 * function：Get Browserr Remote Ipaddress
                 ***************************************************************************/
                std::string remote_ipaddress(bool find_header = false, const std::string& inside_ipaddress = "");
                ushort remote_port();
                friend class reqpack;
            private:
                network::http::reqpack* m_reqpack;
                network::http::session m_session;
                network::http::parser m_parser;
            };
        }
    }
}
#endif
