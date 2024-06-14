/*Software License

Copyright(C) 2024[liuyingjie]
License Terms
Usage Rights

Any individual or entity is free to use, copy, and distribute the binary form of this software without modification to the source code, without the need to disclose the source code.
If the source code is modified, the modifications must be open - sourced under the same license.This means that the modifications must be disclosed and accompanied by a copy of this license.
Future Versions Updates
From this version onwards, all future releases will be governed by the terms of the latest version of the license.This license will automatically be nullified and replaced by the new version.
Users must comply with the terms of the new license issued in future releases.
Liability and Disclaimer
This software is provided “as is”, without any express or implied warranties, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non - infringement.In no event shall the author or copyright holder be liable for any claims, damages, or other liabilities, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
Contact Information
If you have any questions, please contact us: 1585346868@qq.com Or visit our website fwlua.com.
*/

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

	for (size_t i = 0; i < 30; i++)
	{
		if (::ithread::m_state == 1)
			break;
		system::sleep_msec(100);
	}
	
	return true;
}

