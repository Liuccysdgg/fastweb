#include "statemanager.h"

#include "util/file.h"
#include "util/system.h"
#include "util/time.h"

#include "core/define.h"
#include "core/config.h"
#include "core/global.h"

#include "module/http/request.h"
#include "module/http/response.h"
#include "module/http/session.h"
#include "module/http/httpclient.h"
#include "module/mysql.h"
#ifdef _WIN32
#include "module/mssql.h"
#endif
#include "module/localstorage.h"
#include "module/globalfuns.h"
#include "module/mutex.h"
#include "module/codec.h"
#include "module/time.h"
#include "module/file.h"
#define LOOP_STATE_USE 1
bool state_manager::start()
{
#if LOOP_STATE_USE ==  0
	close();
	::ithread::start();
#endif
	return true;
}

void state_manager::close()
{
#if LOOP_STATE_USE ==  0
	::ithread::stop();
	::ithread::wait();
#endif
	sol::state* state = nullptr;
	while (m_states.pop(state))
		delete state;

}
sol::state* state_manager::create_state()
{
	sol::state* lua = new sol::state();
	lua->open_libraries(
		sol::lib::base, 
		sol::lib::package, 
		sol::lib::math, 
		sol::lib::string,
		sol::lib::table,
		sol::lib::utf8,
		sol::lib::bit32,
		sol::lib::coroutine,
		sol::lib::count,
		sol::lib::ffi,
		sol::lib::io,
		sol::lib::jit,
		sol::lib::os
	);
	{
		// 获取当前的package.path，添加新的搜索路径
		std::string current_path = (*lua)["package"]["path"];  // 获取当前的路径
		current_path += ";"+ sConfig->scripts.lib_dir +"/?.lua";  // 添加新的路径
		(*lua)["package"]["path"] = current_path;  // 设置修改后的路径
	}
	module::request::regist(*lua);
	module::response::regist(*lua);
	module::session::regist(*lua);
	module::httpclient::regist(*lua);
	module::mysql_regist(*lua);
	#ifdef _WIN32
	module::mssql::regist(*lua);
	#endif
	module::regist_globalfuns(*lua);
	module::local_storage::regist(lua);
	module::mutex::regist(lua);
	module::auto_lock::regist(lua);
	module::codec::regist(lua);
	module::time::regist(lua);
	module::file::regist(lua);

	global::getInstance()->regist_lua(lua);
	return lua;
}

sol::state* state_manager::get_state()
{
	sol::state* result = nullptr;
	if (m_states.pop(result))
		return result;
	return create_state();
}

void state_manager::push_state(sol::state* state)
{
	if (state == nullptr)
		return;
#if LOOP_STATE_USE == 1
	m_states.push(state);
#else
	m_delete_states.push(state);
#endif
	
	
}


bool state_manager::run()
{
#if LOOP_STATE_USE == 0
	auto now_msec = time::now_msec();
	// 虚拟机缓冲保证
	if(sConfig->scripts.lua_cache_size > m_states.size())
	{
		std::vector<sol::state*> bhvalues;
		auto bhcount = sConfig->scripts.lua_cache_size - m_states.size();
		if (bhcount > 0 && bhcount <= sConfig->scripts.lua_cache_size)
		{
			for (size_t i = 0; i < bhcount; i++)
				bhvalues.push_back(create_state());
		}
		for(size_t i=0;i< bhvalues.size();i++)
			m_states.push(bhvalues[i]);
	}

	// 释放虚拟机
	{
		sol::state* state = nullptr;
		while (m_delete_states.pop(state))
			delete state;
	}

	system::sleep_msec(100);
	return true;
#else
	return false;
#endif
}

