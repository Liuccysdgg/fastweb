#pragma once


#include <map>
#include "base/define.h"
namespace ylib
{
    namespace time
    {
        struct datetime
        {
            datetime() {
                year = 0;
                month = 0;
                day = 0;
                hour = 0;
                minute = 0;
                second = 0;
            }
            short year;
            char month;
            char day;
            char hour;
            char minute;
            char second;
        };

        //网络时间
        timestamp network_msec();

        //标准字符串时间转时间戳
        timestamp to_ts(const std::string& timestr, const std::string& formart = "%d-%d-%d %d:%d:%d");
        timestamp  to_ts(struct ylib::time::datetime& datetime);
        //当前时间戳
        uint32 now_sec();
        timestamp now_msec();
        timestamp now_usec();
        /*

                %a 星期几的简写
                %A 星期几的全称
                %b 月分的简写
                %B 月份的全称
                %c 标准的日期的时间串
                %C 年份的后两位数字
                %d 十进制表示的每月的第几天
                %D 月/天/年
                %e 在两字符域中，十进制表示的每月的第几天
                %F 年-月-日
                %g 年份的后两位数字，使用基于周的年
                %G 年分，使用基于周的年
                %h 简写的月份名
                %H 24小时制的小时
                %I 12小时制的小时
                %j 十进制表示的每年的第几天
                %m 十进制表示的月份
                %M 十时制表示的分钟数
                %n 新行符
                %p 本地的AM或PM的等价显示
                %r 12小时的时间
                %R 显示小时和分钟：hh:mm
                %S 十进制的秒数
                %t 水平制表符
                %T 显示时分秒：hh:mm:ss
                %u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
                %U 第年的第几周，把星期日做为第一天（值从0到53）
                %V 每年的第几周，使用基于周的年
                %w 十进制表示的星期几（值从0到6，星期天为0）
                %W 每年的第几周，把星期一做为第一天（值从0到53）
                %x 标准的日期串
                %X 标准的时间串
                %y 不带世纪的十进制年份（值从0到99）
                %Y 带世纪部分的十进制年份
                %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
                %% 百分号
        取时间字符串*/
        std::string now_time(const std::string& format = "%Y-%m-%d %H:%M:%S");

        std::string format(timestamp time, const std::string& format = "%Y-%m-%d %H:%M:%S");

        void format(timestamp time, struct ylib::time::datetime& datetime);




        ylib::time::datetime now_time2();

        //获取当前时区
        int32 now_zone();
        //获取当前GMT时间
        void now_gmt(std::string& gmt);
        //时间戳转GMT时间
        void to_gmt(const timestamp& timestamp, std::string& gmt);

        //取当前凌晨时间
        timestamp now_zero_sec();
        std::string now_zero_time();
    }
    
}
