#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <map>
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class parser; 
            /*************************************************************************
             * class：FORM表单解析器
             *************************************************************************/
            class form_parser {
            public:
                form_parser();
                ~form_parser();


                std::vector<std::string> names();
                bool get(const std::string& name, form_info& info);
                bool write_file(const std::string& name, const std::string& filepath);

                friend class parser;
            private:
                bool parse(ylib::buffer* data);
                void parse_count(std::vector<uint32>& starts, std::vector<uint32>& lengths);
                void parse_form(uint32 start, uint32 length);
                char getchar(size_t index);
            private:
                ylib::buffer* m_data;
                std::map<std::string, form_info> m_infos;
            };
            /*************************************************************************
             * class：解析器
             *************************************************************************/
            class parser
            {
            public:
                /******************************************************************
                 * function：初始化
                 * param
                 *
                 *      url                     ：           请求地址
                 *      method                  ：           请求类型
                 *      data                    ：           请求数据
                 *      content_type            ：           类型协议头
                 ******************************************************************/
                bool init(const std::string& url, const network::http::method& method, const ylib::buffer& data, const std::string& content_type);
            public:
                parser();
                ~parser();

                const ylib::json& json();
                std::string text();
                /******************************************************************
                 * function：取URL参数
                 * param
                 *      name                    ：           名称
                 *      value                   ：           数据
                 ******************************************************************/
                bool url_param(const std::string& name, std::string& value);
                bool url_param_exist(const std::string& name);
                /******************************************************************
                 * function：取URL请求参数名列表
                 ******************************************************************/
                std::vector<std::string> url_param_names();
                /******************************************************************
                 * function：取BODY参数
                 * param
                 *      name                    ：           名称
                 *      value                   ：           数据
                 ******************************************************************/
                bool body_param(const std::string& name, std::string& value);
                bool body_param_exist(const std::string& name);
                /******************************************************************
                 * function：取BODY请求参数名列表
                 ******************************************************************/
                std::vector<std::string> body_param_names();
                /******************************************************************
                 * function：取BODY请求参数名列表
                 ******************************************************************/
                form_parser* form();

                const std::map<std::string, std::string>& url_param() { return m_url_param; }
                const std::map<std::string, std::string>& body_param() { return m_body_param; }
            private:
                /******************************************************************
                 * function：解析URL
                 ******************************************************************/
                void parse_url(const std::string& url, std::map<std::string, std::string>& map);
                /******************************************************************
                 * function：解析JSON
                 ******************************************************************/
                void parse_json();
                /******************************************************************
                 * function：解析FORM表单
                 ******************************************************************/
                void parse_form();
                /******************************************************************
                 * function：解析URL格式数据
                 ******************************************************************/
                bool read_url_param(const std::map<std::string, std::string>& map, const std::string& name, std::string& value);


 
            private:
                // URL参数
                std::map<std::string, std::string> m_url_param;
                // BODY参数
                std::map<std::string, std::string> m_body_param;
                // JSON参数
                ylib::json m_json_param;
                // FORM解析器
                network::http::form_parser m_form;
                // 请求地址
                std::string m_url;
                // 请求数据
                ylib::buffer m_data;
                // 请求内容类型
                std::string m_content_type;
                // 请求类型
                network::http::method m_method;

            };
        }
    }
}
#endif
