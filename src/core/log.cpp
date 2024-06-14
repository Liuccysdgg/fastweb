/*Software License

Copyright(C) 2024[liuyingjie]
License Terms
Usage Rights

Any individual or entity is free to use, copy, and distribute the binary form of this software without modification to the source code, without the need to disclose the source code.
If the source code is modified, the modifications must be open - sourced under the same license.This means that the modifications must be disclosed and accompanied by a copy of this license.
Future Versions Updates
From this version onwards, all future releases will be governed by the terms of the latest version of the license.This license will automatically be nullified and replaced by the new version.
Users must comply with the terms of the new license issued in future releases.
Liability and Disclaimer
This software is provided “as is”, without any express or implied warranties, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non - infringement.In no event shall the author or copyright holder be liable for any claims, damages, or other liabilities, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
Contact Information
If you have any questions, please contact us: 1585346868@qq.com Or visit our website fwlua.com.
*/

#include "log.h"
#include "util/print.h"
#include "util/time.h"
#include "util/codec.h"
#include "util/file.h"
#include "core/define.h"
#include "core/config.h"
#include "core/app.h"
#ifdef _WIN32
#include <Windows.h>
#endif
#define DEBUG_INFO 0
std::string escapeString(const std::string& str) {
    std::string escapedStr = str;
    size_t pos = 0;
    while ((pos = escapedStr.find("'", pos)) != std::string::npos) {
        escapedStr.insert(pos, "'");
        pos += 2;
    }
    return escapedStr;
}

inline std::string typestring(fastweb::log::log_type type)
{
    std::string type_str;
    switch (type)
    {
    case fastweb::log::LT_SUCCESS:
        type_str = "[SUCC ] ";
        break;
    case fastweb::log::LT_INFO:
        type_str = "[INFO ] ";
        break;
    case fastweb::log::LT_ERROR:
        type_str = "[ERROR] ";
        break;
    case fastweb::log::LT_WARN:
        type_str = "[WARN ] ";
        break;
    case fastweb::log::LT_DEBUG:
        type_str = "[DEBUG] ";
        break;
    case fastweb::log::LT_LUA:
        type_str = "[LUA  ] ";
        break;
    default:
        type_str = "[NONE ] ";
        break;
    }
    return type_str;
}
void fastweb::log::print(log_type type,const std::string& msg,const std::string& filepath, const std::string& func, int line,int color,bool error) {

    std::string typestr = typestring(type);
    std::string __now_time = time::now_time("%Y-%m-%d %H:%M:%S");
    printf("[%s] ", __now_time.c_str());
#if DEBUG_INFO == 1
    printf("%s", ylib::file::filename(filepath).c_str());
    printf("%s ", std::string(":" + func + ":" + std::to_string(line) + "\t").c_str());
#endif
    ylib::print(" "+ typestr +" ", (ylib::ConsoleTextColor)color);

#ifdef _WIN32
    ylib::println(codec::to_gbk(msg), (ylib::ConsoleTextColor)color);
#else
    ylib::println(msg, (ylib::ConsoleTextColor)color);
#endif

    log_info li;
    li.create_at = __now_time;
    li.filepath = filepath;
    li.function = func;
    li.line = line;
    li.msg = msg;
    li.type = type;
    m_queue.push(li);
}
bool fastweb::log::run()
{

    if (app()->config == nullptr)
    {
        ITHREAD_WAIT_MSEC(1000);
        return true;
    }
    if (app()->config->log.enable == false)
    {
        ITHREAD_WAIT_MSEC(1000);
        return true;
    }
   
    if (write() == false)
        return false;

    ITHREAD_WAIT_MSEC(1000);
    return true;
}
fastweb::log::log(fastweb::app* ptr):Interface(ptr)
{
    ::ithread::start();
    
}
fastweb::log::~log()
{
    ::ithread::stop();
    ::ithread::wait();
}
void fastweb::log::start()
{

}
void fastweb::log::success(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    //log_error();
    this->print(LT_SUCCESS ,msg, filepath, func, line, ylib::ConsoleTextColor::GREEN 
    #ifdef _WIN32
    |FOREGROUND_INTENSITY
    #endif
    ,false);
}
void fastweb::log::info(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    this->print(LT_INFO,msg, filepath, func, line, ylib::ConsoleTextColor::GREEN | ylib::ConsoleTextColor::RED | ylib::ConsoleTextColor::BLUE, false);
    
}
void fastweb::log::error(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    this->print(LT_ERROR, msg, filepath, func, line, ylib::ConsoleTextColor::RED
    #ifdef _WIN32
    | FOREGROUND_INTENSITY
    #endif
        , true);
}

void fastweb::log::warn(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    this->print(LT_WARN, msg, filepath, func, line, ylib::ConsoleTextColor::YELLOW
    #ifdef _WIN32
    | FOREGROUND_INTENSITY
    #endif
        , false);
}

void fastweb::log::debug(const std::string& msg, const std::string& filepath, const std::string& func,int line)
{
    this->print(LT_DEBUG, msg, filepath, func, line, ylib::ConsoleTextColor::BLUE
    #ifdef _WIN32
    | FOREGROUND_INTENSITY
    #endif
        , false);
}

void fastweb::log::lua(const std::string& msg)
{
    this->print(LT_LUA, msg,"","",0, ylib::ConsoleTextColor::GREEN | ylib::ConsoleTextColor::RED | ylib::ConsoleTextColor::BLUE, false);
}

bool fastweb::log::write()
{
    std::string logcontent;
    std::string new_time = time::now_time(app()->config->log.name);
    if (new_time == "")
    {
        std::cout << "error: The log file name is incorrect: " << app()->config->log.name << std::endl;
        return false;
    }
    if (m_current_name != new_time)
    {
        m_current_name = new_time;
        ylib::file::create_dir(app()->config->log.dir, true);
        std::string newfilepath = app()->config->log.dir + "/" + new_time;
        m_file.close();
        if (m_file.open(newfilepath) == false)
        {
            std::cout << "error: open log file failed, filepath: " << newfilepath << std::endl;
            return false;
        }
        if (app()->config->log.sqlite)
        {
            if (m_sqlite.is_open() == false)
            {
                m_sqlite.open(app()->config->log.dir + "/log.db");
                m_sqlite.exec(R"(CREATE TABLE "log" (
                    "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
                    "type" INTEGER,
                    "create_at" TEXT,
                    "content" TEXT,
                    "flag" integer
                ); )");
            }
            
        }
    }
    log_info li;
    while (m_queue.pop(li))
    {
        std::string typestr = typestring(li.type);
        std::string logcontent = li.create_at + " " + typestr + " " + li.msg
#ifdef _WIN32
            + "\r\n";
#else
            + "\n";
#endif
        m_file.appead(logcontent);
        if(app()->config->log.sqlite)
            m_sqlite.exec("INSERT INTO log(type,create_at,content,flag)VALUES(" + std::to_string(li.type) + ",'"+li.create_at+"','"+ escapeString(li.msg)+"',0)");
    }
    return true;
}
