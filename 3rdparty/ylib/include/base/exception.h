#pragma once

#include <string>
#include <exception>

#include "base/define.h"

/**
 * @brief 异常处理类
 */
namespace ylib
{
    class exception:public std::exception
    {
    public:
        exception(const std::string &msg);
        ~exception();
        virtual char const* what() const 
#ifdef __linux__
            noexcept
#endif
            override{return m_msg.c_str();}
    private:
        std::string m_msg;
    };
}


