#pragma once
#include "net/http_request.h"
#include "net/http_response.h"
#include "sol/sol.hpp"

namespace module
{
    class interceptor
    {
    public:
        interceptor();
        ~interceptor();
        /// <summary>
        /// 添加
        /// </summary>
        /// <param name="express_string"></param>
        /// <param name="filepath"></param>
        /// <returns></returns>
        static bool add(const std::string& express_string,const std::string& filepath, sol::this_state ts);
        /// <summary>
        /// 移除
        /// </summary>
        /// <param name="express_string"></param>
        static bool remove(const std::string& express_string, sol::this_state ts);
        /// <summary>
        /// 是否存在
        /// </summary>
        /// <param name="express_string"></param>
        /// <returns></returns>
        static bool exist(const std::string& express_string, sol::this_state ts);
        /// <summary>
        /// 清空
        /// </summary>
        static void clear(sol::this_state ts);
       
        static void regist(sol::state* lua);
    };
}

