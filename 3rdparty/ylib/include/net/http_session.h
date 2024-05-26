#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include <map>
#include <string>
#include <vector>
#include "util/counter.hpp"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class website;
            class server;
            /// <summary>
            /// 会话数据
            /// </summary>
            class session
            {
            public:
                session();
                ~session();

                /// <summary>
                /// 初始化
                /// </summary>
                /// <param name="id"></param>
                void init(network::http::website* website,const std::string& id);
                /// <summary>
                /// 唯一ID
                /// </summary>
                /// <returns></returns>
                std::string id();
                /// <summary>
                /// 更新(防止过期)
                /// </summary>
                void update();
                /// <summary>
                /// 置数据
                /// </summary>
                /// <param name="name"></param>
                /// <param name="value"></param>
                void set(const std::string& name,const std::string& value);
                /// <summary>
                /// 读数据
                /// </summary>
                /// <param name="name"></param>
                /// <returns></returns>
                std::string get(const std::string& name);
                /// <summary>
                /// 校验是否过期
                /// </summary>
                /// <returns></returns>
                bool check();
            private:
                // 唯一ID
                std::string m_id;
                // 网站
                network::http::website* m_website = nullptr;
            };
        }
    }
}



#endif
