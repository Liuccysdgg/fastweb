#pragma once
#include <string>
namespace LuaUtils {

	/// <summary>
	/// 创建服务字节码
	/// </summary>
	/// <param name="filepath"></param>
	/// <param name="bytecode"></param>
	/// <returns></returns>
	bool make_bytecode(const std::string& filepath,std::string& bytecode);
}