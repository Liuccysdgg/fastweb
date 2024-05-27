#pragma once
#include "core/define.h"
/// <summary>
/// 全局注册函数
/// </summary>
namespace module
{
	void regist_globalfuns(sol::state& lua);
	/// <summary>
	/// 生成软件唯一GUID
	/// </summary>
	/// <returns></returns>
	std::string make_software_guid();
	/// <summary>
	/// 注册全局指针(仅允许初始化lua使用)
	/// </summary>
	/// <returns></returns>
	bool global_regist(const std::string& name, void* ptr, sol::this_state ts);

	void global_set(const std::string& name, VarType value);
	VarType global_get(const std::string& name, sol::this_state s);
	/// <summary>
	/// 抛出异常
	/// </summary>
	/// <param name="msg"></param>
	void throw_string(const std::string& msg);
}

