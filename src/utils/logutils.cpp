#include "logutils.h"
#include "util/print.h"
#include "util/time.h"
#include "util/codec.h"
#include "util/file.h"
#include <Windows.h>
#define DEBUG_INFO 0
void print(const std::string& type,const std::string& msg,const std::string& filepath, const std::string& func, int line,int color) {

    printf("[%s] ", time::now_time("%m-%d %H:%M:%S").c_str());
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

}
void LogUtils::success(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    ::print("[SUCC ] ", msg, filepath, func, line, ylib::ConsoleTextColor::GREEN | FOREGROUND_INTENSITY);
}

void LogUtils::info(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    ::print("[INFO ] ", msg, filepath, func, line, ylib::ConsoleTextColor::GREEN | ylib::ConsoleTextColor::RED | ylib::ConsoleTextColor::BLUE);
    
}

void LogUtils::error(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
#ifdef _DEBUG
    ::print("[ERROR] ", msg, filepath, func, line, ylib::ConsoleTextColor::RED | FOREGROUND_INTENSITY);
#else
    
#endif
}

void LogUtils::warn(const std::string& msg, const std::string& filepath, const std::string& func, int line)
{
    ::print("[WARN ] ", msg, filepath, func, line, ylib::ConsoleTextColor::YELLOW | FOREGROUND_INTENSITY);
}

void LogUtils::debug(const std::string& msg, const std::string& filepath, const std::string& func,int line)
{
    ::print("[DEBUG] ", msg, filepath, func, line, ylib::ConsoleTextColor::BLUE | FOREGROUND_INTENSITY);
}
