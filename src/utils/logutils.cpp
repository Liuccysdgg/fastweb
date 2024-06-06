#include "logutils.h"
#include "util/print.h"
#include "util/time.h"
#include "util/codec.h"
#include "util/file.h"
#include "core/define.h"
#include "core/config.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#define DEBUG_INFO 0
void LogUtils::print(const std::string& type,const std::string& msg,const std::string& filepath, const std::string& func, int line,int color,bool error) {

    std::string __now_time = time::now_time("%m-%d %H:%M:%S");
    printf("[%s] ", __now_time.c_str());
#if DEBUG_INFO == 1
    printf("%s", ylib::file::filename(filepath).c_str());
    printf("%s ", std::string(":" + func + ":" + std::to_string(line) + "\t").c_str());
#endif
    ylib::print(" "+type +" ", (ylib::ConsoleTextColor)color);

#ifdef _WIN32
    ylib::println(codec::to_gbk(msg), (ylib::ConsoleTextColor)color);
#else
    ylib::println(msg, (ylib::ConsoleTextColor)color);
#endif

    if (sConfig->log.enable)
    {
        std::string logcontent = __now_time + " " + type + " " + msg
#ifdef _WIN32
            + "\r\n";
#else
            + "\n";
#endif
        std::string new_time = time::now_time(sConfig->log.name);
        if (new_time == "")
        {
            std::cout << "error: The log file name is incorrect: " << sConfig->log.name << std::endl;
            return;
        }

        if (m_current_name != new_time)
        {
            ylib::file::create_dir(sConfig->log.dir, true);
            std::string newfilepath = sConfig->log.dir + "/" + new_time;
            m_file.close();
            if (m_file.open(newfilepath) == false)
            {
                std::cout << "error: open log file failed, filepath: " << newfilepath << std::endl;
                return;
            }
        }
        m_file.appead(logcontent);
    }
}
LogUtils::LogUtils()
{

}
LogUtils::~LogUtils()
{
}
void LogUtils::success(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    //log_error();
    this->print("[SUCC ] ", msg, filepath, func, line, ylib::ConsoleTextColor::GREEN 
    #ifdef _WIN32
    |FOREGROUND_INTENSITY
    #endif
    ,false);
}

void LogUtils::info(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    this->print("[INFO ] ", msg, filepath, func, line, ylib::ConsoleTextColor::GREEN | ylib::ConsoleTextColor::RED | ylib::ConsoleTextColor::BLUE, false);
    
}

void LogUtils::error(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
//#ifdef _DEBUG
    this->print("[ERROR] ", msg, filepath, func, line, ylib::ConsoleTextColor::RED
    #ifdef _WIN32
    | FOREGROUND_INTENSITY
    #endif
        , true);
//#else
    
//#endif
}

void LogUtils::warn(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    this->print("[WARN ] ", msg, filepath, func, line, ylib::ConsoleTextColor::YELLOW
    #ifdef _WIN32
    | FOREGROUND_INTENSITY
    #endif
        , false);
}

void LogUtils::debug(const std::string& msg, const std::string& filepath, const std::string& func,int line)
{
    this->print("[DEBUG] ", msg, filepath, func, line, ylib::ConsoleTextColor::BLUE
    #ifdef _WIN32
    | FOREGROUND_INTENSITY
    #endif
        , false);
}
