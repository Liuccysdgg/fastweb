#pragma once


#include "sol/sol.hpp"

#include "base/error.h"
#include "base/singleton.hpp"
#include "util/thread.h"
#include "util/queue.hpp"
#include "util/map.hpp"

#include "core/structs.h"
#include "core/lualibdetecter.h"
#include "core/modulemanager.h"
/// <summary>
/// LUA状态管理器
/// </summary>
class state_manager:public ylib::singleton<state_manager>,private ylib::ithread,public ylib::error_base {
public:
	state_manager() = default;
	
	/// <summary>
	/// 启动
	/// </summary>
	bool start();
	void close();
	/// <summary>
	/// 取虚拟机
	/// </summary>
	/// <returns></returns>
	luastate* get();
	/// <summary>
	/// 归还虚拟机
	/// </summary>
	/// <param name="state"></param>
	void push(luastate* state);
private:
	// 虚拟机
	ylib::queue<luastate*> m_states;
	// 版本FLAT
	size_t m_flag = 0;
	// LIB变化检测
	lualib_detecter m_lib_detecter;
	// 模块管理器
	module_manager m_module_manager;
private:
	// 通过 ithread 继承
	bool run() override;
	/// <summary>
	/// 创建虚拟机
	/// </summary>
	/// <returns></returns>
	luastate* create();




};