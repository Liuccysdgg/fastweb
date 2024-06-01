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
#include "module/sys.h"
#define LOOP_STATE_USE 1
bool state_manager::start()
{
	close();
	::ithread::start();
	return true;
}

void state_manager::close()
{
	::ithread::stop();
	::ithread::wait();
	luastate* state = nullptr;
	while (m_states.pop(state))
		delete state;
}
luastate* state_manager::create()
{
	luastate* lua = new luastate();
	lua->flag = m_flag;
	lua->state->open_libraries(
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
		std::string current_path = (*lua->state)["package"]["path"];  // 获取当前的路径
		current_path += ";"+ sConfig->scripts.lib_dir +"/?.lua";  // 添加新的路径
		(*lua->state)["package"]["path"] = current_path;  // 设置修改后的路径
	}
	module::request::regist(*lua->state);
	module::response::regist(*lua->state);
	module::session::regist(*lua->state);
	module::httpclient::regist(*lua->state);
	module::mysql_regist(*lua->state);
	#ifdef _WIN32
	module::mssql::regist(*lua->state);
	#endif
	module::regist_globalfuns(*lua->state);
	module::local_storage::regist(lua->state);
	module::mutex::regist(lua->state);
	module::auto_lock::regist(lua->state);
	module::codec::regist(lua->state);
	module::time::regist(lua->state);
	module::file::regist(lua->state);
	module::sys::regist(lua->state);

	global::getInstance()->regist_lua(lua->state);
	return lua;
}

luastate* state_manager::get()
{
	luastate* result = nullptr;
	while (m_states.pop(result))
	{
		if (result->flag != m_flag)
			delete result;
		else
			return result;
	}
	return create();
}

void state_manager::push(luastate* state)
{
	if (state == nullptr)
		return;
	if (state->flag != m_flag)
	{
		delete state;
		return;
	}
	m_states.push(state);
}


bool state_manager::run()
{
	if (m_lib_detecter.changed())
		m_flag++;
	system::sleep_msec(sConfig->scripts.auto_update_sec);
	return true;
}

