#pragma once
#include "base/define.h"
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