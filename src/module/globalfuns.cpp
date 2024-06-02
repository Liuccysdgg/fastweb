#include "globalfuns.h"
#include "util/counter.hpp"
#include "util/codec.h"
#include "util/time.h"
#include "core/global.h"
static ylib::counter<uint64> s_counter_guid;
void module::regist_globalfuns(sol::state* lua)
{
	lua->set_function("global_get", module::global_get);
	lua->set_function("global_set", module::global_set);
	lua->set_function("make_software_guid", module::make_software_guid);
	lua->set_function("throw_string", module::throw_string);
}
std::string module::make_software_guid()
{
	return codec::md5(std::to_string(time::now_msec()) + std::to_string(s_counter_guid.make()));
}

bool module::global_regist(const std::string& name, void* ptr, sol::this_state ts)
{
	return global::getInstance()->regist_ptr(name,ptr, ts);
}

void module::global_set(const std::string& name, VarType value)
{
	global::getInstance()->set(name,value);
}

VarType module::global_get(const std::string& name, sol::this_state s)
{
	return global::getInstance()->get(name,s);
}

#if 0
void* module::global_get(const std::string& name)
{
	return global::getInstance()->get_ptr(name);
}
#endif
void module::throw_string(const std::string& msg)
{
	throw ylib::exception(msg);
}
