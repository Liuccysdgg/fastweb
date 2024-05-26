#pragma once


#include "sol/sol.hpp"

#include "base/error.h"
#include "base/singleton.hpp"
#include "util/thread.h"
#include "util/queue.hpp"
#include "util/map.hpp"

#include "core/structs.h"

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
	sol::state* get_state();
	/// <summary>
	/// 归还虚拟机
	/// </summary>
	/// <param name="state"></param>
	void push_state(sol::state* state);
private:
	// 虚拟机
	ylib::queue<sol::state*> m_states;
	ylib::queue<sol::state*> m_delete_states;
private:
	// 通过 ithread 继承
	bool run() override;

	sol::state* create_state();


};