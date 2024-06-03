#include "statemanager.h"

#include "util/file.h"
#include "util/system.h"
#include "util/time.h"

#include "core/define.h"
#include "core/config.h"
#include "core/global.h"


#define LOOP_STATE_USE 1
bool state_manager::start()
{
	close();
	::ithread::start();
	m_module_manager.start();
	return true;
}

void state_manager::close()
{
	::ithread::stop();
	::ithread::wait();
	luastate* state = nullptr;
	while (m_states.pop(state))
		delete state;
	m_module_manager.close();
}
luastate* state_manager::create()
{
	luastate* lua = new luastate();
	lua->flag = m_flag;
	// 加载库或模块
	m_module_manager.load(lua->state);
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
	system::sleep_msec(sConfig->scripts.auto_update_sec*1000);
	return true;
}

