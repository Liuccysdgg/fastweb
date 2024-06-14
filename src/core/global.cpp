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


#include "global.h"
#include "module/basemodule.h"
fastweb::global::global(fastweb::app* app):Interface(app)
{
}
fastweb::global::~global()
{
	clear();
}

void fastweb::global::regist(sol::state* lua)
{
	m_ptrs.lock();
	for_iter(iter, (*m_ptrs.parent()))
	{
		auto im = static_cast<module::base*>(iter->second);
		im->regist_global(iter->first.c_str(), lua);
	}
	m_ptrs.unlock();
}
void* fastweb::global::get_ptr(const std::string& name)
{
	void* result = nullptr;
	m_ptrs.get(name,result);
	return result;
}

bool fastweb::global::set_ptr(const std::string& name, void* value, sol::this_state ts)
{
	sol::state_view lua(ts);
	// INIT中先注册一次，防止被销毁
	{
		lua.registry()[name] = this;
		lua[name] = this;
	}
	return m_ptrs.add(name, value);
}

sol::object fastweb::global::get_str(const std::string& name, sol::this_state s)
{
	std::string value;
	if (m_values.get(name, value))
		return  sol::make_object(s, value);
	return sol::make_object(s, sol::nil);
}

void fastweb::global::set_str(const std::string& name, std::string value)
{
	m_values.set(name, value, true);
}

void fastweb::global::clear()
{
	//for_iter(iter, (*m_ptrs.parent()))
	//{
	//	//auto base = ((module::base*)iter->second);
	//	//base->delete_global();
	//}
	m_ptrs.clear();
	m_values.clear();
}
