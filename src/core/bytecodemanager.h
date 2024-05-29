#pragma once

#include "core/define.h"
#if ENABLE_BYTECODE == 1
#include "sol/sol.hpp"

#include "base/error.h"
#include "base/singleton.hpp"
#include "util/thread.h"
#include "util/queue.hpp"
#include "util/map.hpp"

#include "core/structs.h"

/// <summary>
/// 字节码管理器
/// </summary>
class bytecode_manager:public ylib::error_base,private ylib::ithread {
public:
	bytecode_manager();
	~bytecode_manager();
	/// <summary>
	/// 创建字节码
	/// </summary>
	/// <param name="name"></param>
	/// <param name="filepath"></param>
	/// <param name="auto_update"></param>
	/// <returns></returns>
	bool create(const std::string& name,const std::string& filepath,bool auto_update);
	/// <summary>
	/// 取字节码
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	const std::string& get(const std::string& name);

	std::map<std::string, std::shared_ptr<bytecode>> map();
private:
	// ServiceLUA文件
	ylib::map<std::string, std::shared_ptr<bytecode>> m_bytescodes;

	// 通过 ithread 继承
	bool run() override;
};
#endif