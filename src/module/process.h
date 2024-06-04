#pragma once
#include "sol/sol.hpp"

namespace module
{
	/// <summary>
	/// 进程
	/// </summary>
	class process{
	public:
		/// <summary>
		/// 创建
		/// </summary>
		/// <param name="filepath"></param>
		/// <param name="working_directory"></param>
		/// <param name="args"></param>
		/// <param name="wait_close"></param>
		/// <param name="show_window"></param>
		/// <returns>0=失败 否则为PID</returns>
		static size_t create(const std::string & filepath, const std::string & working_directory,sol::table args, bool wait_close, bool show_window);
		/// <summary>
		/// 关闭
		/// </summary>
		/// <param name="pid"></param>
		/// <returns></returns>
		static bool destory(size_t pid);
		/// <summary>
		/// 进程列表
		/// </summary>
		/// <param name="s"></param>
		/// <returns></returns>
		static sol::table list(sol::this_state s);
		/// <summary>
		/// 是否存在
		/// </summary>
		/// <param name="filepath"></param>
		/// <returns></returns>
		static size_t exist(const std::string& filepath);
		static bool exist_pid(size_t pid);
		/// <summary>
		/// 取PID路径
		/// </summary>
		/// <param name="pid"></param>
		/// <returns></returns>
		static std::string getpath(size_t pid);


		static void regist(sol::state* lua);
	};

}

