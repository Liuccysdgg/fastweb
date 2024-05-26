#pragma once
#include "http_define.h"
#if USE_NET_HTTP_UTIL
#include <string>
#include <list>
namespace ylib
{
    namespace network
    {
        //取文档类型
        void content_type(const std::string& extName, std::string& type);
        //拆分URL
        bool parse_url(const std::string& url, std::string& httpType, std::string& host, std::string& ipaddress, ushort& port, std::string& urlField);
        //拆分URL
        bool parse_url_host(const std::string& url, std::string& host);
        //INT IP 转 字符串IP
        void to_string(uint32 int_ip, std::string& ipaddress);
        struct TcpConf
        {
            TcpConf()
            {
                local_port = 0;
                remote_port = 0;
                __local_ipaddress = 0;
                __remote_ipaddress = 0;
            }
            void local_ipaddress(std::string& ipaddress) const
            {
                to_string(__local_ipaddress, ipaddress);
            }
            void remote_ipaddress(std::string& ipaddress) const
            {
                to_string(__remote_ipaddress, ipaddress);
            }
            uint32 __local_ipaddress;
            uint32 __remote_ipaddress;
            uint32 local_port;
            uint32 remote_port;
        };

        //URL转IP地址
        std::string  to_ip(const std::string& url);
        // 端口是否占用
        bool is_occupy(uint32 port);
        /*
            取大小名称
            B/KB/MB/GB
        */
        std::string size_name(double size, uint32 fixe = 0);
        // 是否为IPV4
        bool is_ipv4(const std::string& value);
        // 是否为IPV6
        bool is_ipv6(const std::string& value);
        // 是否为域名
        bool is_domain(const std::string& value);
    }
}
#endif
