// MIT License﻿
// Copyright(c) 2024 FastWeb - fwlua.com - nianhua﻿
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :﻿
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.﻿
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.﻿
// ## Additional Terms for Commercial Use﻿
// This software is licensed for personal, educational, and non - commercial use.
// For commercial use or use within a company, organization, or institution, a
// separate commercial license is required.To obtain a commercial license,
// please contact
// EMail：1585346868@qq.com

// QQ：1585346868

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

