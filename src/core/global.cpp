
#include "global.h"
#include "module/imodule.h"
void global::regist_lua(sol::state* lua)
{
	m_value_ptr.lock();
	for_iter(iter, (*m_value_ptr.parent()))
	{
		auto im = static_cast<module::imodule*>(iter->second);
		im->regist_global(iter->first, lua);
	}
	m_value_ptr.unlock();
}
void* global::get_ptr(const std::string& name)
{
	void* result = nullptr;
	m_value_ptr.get(name,result);
	return result;
}

bool global::regist_ptr(const std::string& name, void* value, sol::this_state ts)
{
	sol::state_view lua(ts);
	// INIT中先注册一次，防止被销毁
	{
		lua.registry()[name] = this;
		lua[name] = this;
	}
	return m_value_ptr.add(name, value);
}
