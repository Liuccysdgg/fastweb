
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
		im->regist_global(iter->first, lua);
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

sol::object fastweb::global::get_obj(const std::string& name, sol::this_state s)
{
	sol::object value;
	if (m_values.get(name, value))
	{
		return value;
	}
	return sol::make_object(s, sol::nil);
}

void fastweb::global::set_obj(const std::string& name, sol::object value)
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
