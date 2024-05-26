#pragma once

#include <string>
#include <iostream>
/**
 * 基础错误类
 */

namespace ylib
{
    class error_base
    {
    public:
        const std::string& last_error();
    public:
        std::string m_lastErrorDesc;
    };

    namespace error{
        void print(const std::string& value);
    }
}

