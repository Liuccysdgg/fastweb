﻿#pragma once
#include "core/define.h"
/// <summary>
/// 全局注册函数
/// </summary>
namespace module
{
	class globalfuncs {
	public:
		/// <summary>
		/// 生成软件唯一GUID
		/// </summary>
		/// <returns></returns>
		static std::string make_software_guid();
		/// <summary>
		/// 接管打印
		/// </summary>
		/// <param name="args"></param>
		static void print(sol::variadic_args args, sol::this_state ts);
		/// <summary>
		/// 置全局指针
		/// </summary>
		/// <param name="name"></param>
		/// <param name="ptr"></param>
		/// <param name="ts"></param>
		/// <returns></returns>
		static bool set_ptr(const std::string& name, void* ptr, sol::this_state ts);

		/// <summary>
		/// 置全局文本
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		static void set_str(const std::string& name,std::string value, sol::this_state ts);
		/// <summary>
		/// 取全局文本
		/// </summary>
		/// <param name="name"></param>
		/// <param name="s"></param>
		/// <returns></returns>
		static sol::object get_str(const std::string& name, sol::this_state s);
		/// <summary>
		/// 抛出异常
		/// </summary>
		/// <param name="msg"></param>
		static void throw_string(const std::string& msg);
		/// <summary>
		/// 网站目录
		/// </summary>
		static std::string website_dir(sol::this_state ts);
		/// <summary>
		/// 创建环境
		/// </summary>
		/// <returns></returns>
		static void create_env(const std::string& lua_filepath,sol::this_state ts);

		static void regist(sol::state* lua);
	};
	
}

