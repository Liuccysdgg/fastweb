#include "statemanager.h"

#include "util/file.h"
#include "util/system.h"
#include "util/time.h"

#include "core/define.h"
#include "core/config.h"
#include "core/global.h"
#include "core/app.h"


#define LOOP_STATE_USE 1
fastweb::state_manager::state_manager(fastweb::app* app):Interface(app)
{
	lib_detecter = std::make_shared<fastweb::lualib_detecter>(app);
	module_manager = std::make_shared<fastweb::module_manager>(app);
}
fastweb::state_manager::~state_manager()
{
}
bool fastweb::state_manager::start()
{
	close();
	::ithread::start();
	module_manager->start();
	return true;
}

void fastweb::state_manager::close()
{
	::ithread::stop();
	::ithread::wait();
	luastate* state = nullptr;
	while (m_states.pop(state))
		delete state;
	module_manager->close();
}
luastate* fastweb::state_manager::create()
{
	luastate* lua = new luastate();
	lua->flag = m_flag;
	(*lua->state)["____app"] = app();

	// 加载库或模块
	module_manager->load(lua->state);
	return lua;
}

luastate* fastweb::state_manager::get()
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

void fastweb::state_manager::push(luastate* state)
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


bool fastweb::state_manager::run()
{
	if (lib_detecter->changed())
		m_flag++;
	system::sleep_msec(3000);
	return true;
}

