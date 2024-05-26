#pragma once
#include <vector>
#include "base/define.h"
namespace ylib
{ 
	namespace system
	{
		//睡眠
		void sleep_msec(uint32 msec);
		//取随机数
		int64 random(int64 min, int64 max);
		//取运行目录
		std::string current_dir();
		//取运行全路径
		std::string current_filepath();
		//取临时目录
		std::string temp_path();
		//桌面路径 
		std::string desktop_path();
		//取当前用户目录
		std::string currentuser_path();
		//系统通知
		void notification(const std::string& title, const std::string& message);
		//mac地址
		std::vector<std::string> mac();
#ifdef _WIN32
		//打开浏览器
		void open_browser(const std::string& url);
		//磁盘列表
		std::vector<char> disk_list();
		//取磁盘信息
		ylib::DiskCapacity disk_capacity(char disk_name);
		//取容量最大磁盘号
		char disk_max_capacity();
		//系统版本
		std::string version();
		//取系统最后错误描述
		std::string last_error();
#endif
	}
	
	
}

