#pragma once
#include "define.h"

namespace ylib
{
/*
     * @class：Log4cplus 日志封装类
     * @desc：
     *      1、程序默认开启，如需更改请修改 public/environment.cpp 中的全局变量实现(注释new即可)
     *      2、默认配置文件放置程序目录 res/log.properties, 示例文件参考：res/log.properties
     */
class log4
{
public:
    log4(const std::string& config_filepath);
    ~log4();
    ylib::log4& info(const std::string& value,const std::string& name = "ALL");
    ylib::log4& error(const std::string& value,const std::string& name = "ALL");
    ylib::log4& fatal(const std::string& value,const std::string& name = "ALL");
    ylib::log4& warn(const std::string& value,const std::string& name = "ALL");
    ylib::log4& debug(const std::string& value,const std::string& name = "ALL");
private:
    void* m_appender;
    void* m_appender_console;
};
}
