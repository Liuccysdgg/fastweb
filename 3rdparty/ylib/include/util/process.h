#pragma once
#include <functional>
#include <list>
#include <string>
#include <vector>
#include "base/define.h"
namespace ylib
{
	namespace process
	{
#ifdef _WIN32
		// 取进程路径
		std::string getpath(uint32 process_id);
		/*进程信息*/
		struct proc_info
		{
			std::string path()
			{
				if (__filepath.empty())
					return ylib::process::getpath(pid);
				return __filepath;
			}
			std::string __filepath;
			std::string name;
			uint32 parent_pid = 0;
			uint32 pid = 0;
		};
		// 创建进程
		bool create(const std::string& filepath, const std::string& working_directory = "", const std::vector<std::string>& args = {}, bool wait_close = false, bool show_window = true, size_t* pid = nullptr);
		// 销毁进程 
		bool destory(uint32 process_id);
		// 系统进程列表
		std::list<ylib::process::proc_info> list();
		// 是否存在
		bool exist(const std::string& filepath);
		// 检测多开
		bool already_running(const std::string& name);
		// 设置为启动
		bool running(const std::string& name);
#endif
	}

}
