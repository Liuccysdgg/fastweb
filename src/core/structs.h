#pragma once
#include "base/define.h"
#include "sol/sol.hpp"
/// <summary>
/// LUA字节码
/// </summary>
struct bytecode {
	// 路径
	std::string filepath;
	// 上次修改时间
	timestamp pre_modify_msec = 0;
	// 字节码
	std::string value;
	// 自动更新
	bool auto_update = false;
};
/// <summary>
/// 包装虚拟机
/// </summary>
struct luastate {
	luastate()
	{
		state = new sol::state();
	}
	~luastate()
	{
		delete state;
	}
	sol::state* state = nullptr;
	size_t flag = 0;
};