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
