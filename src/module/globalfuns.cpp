#include "globalfuns.h"
#include "util/counter.hpp"
#include "util/codec.h"
#include "util/time.h"
#include "core/global.h"
#include "core/app.h"
static ylib::counter<uint64> s_counter_guid;
void module::globalfuncs::regist(sol::state* lua)
{
	lua->set_function("set_ptr", module::globalfuncs::set_ptr);
	lua->set_function("get_str", module::globalfuncs::get_str);
	lua->set_function("set_str", module::globalfuncs::set_str);
	lua->set_function("make_software_guid", module::globalfuncs::make_software_guid);
	lua->set_function("throw_string", module::globalfuncs::throw_string);
	lua->set_function("print", module::globalfuncs::print);

}
std::string module::globalfuncs::make_software_guid()
{
	return codec::md5(std::to_string(time::now_msec()) + std::to_string(s_counter_guid.make()));
}
void module::globalfuncs::print(sol::variadic_args args, sol::this_state ts)
{
	GET_APP;

	std::ostringstream oss;
	for (auto arg : args) {
		if (arg.get_type() == sol::type::string) {
			oss << arg.as<std::string>();
		}
		else if (arg.get_type() == sol::type::number) {
			oss << arg.as<double>(); // 可以处理整数和浮点数
		}
		else if (arg.get_type() == sol::type::boolean) {
			oss << (arg.as<bool>() ? "true" : "false");
		}
		else if (arg.get_type() == sol::type::nil) {
			oss << "nil";
		}
		else {
			oss << "unsupported type";
		}
		oss << " "; // 添加一个空格分隔符
	}
	app->log->lua(oss.str());
}
bool module::globalfuncs::set_ptr(const std::string& name, void* ptr, sol::this_state ts)
{
	GET_APP;
	return app->global->set_ptr(name,ptr, ts);
}

void module::globalfuncs::set_str(const std::string& name, std::string value, sol::this_state ts)
{
	GET_APP;
	return app->global->set_str(name,value);
}

sol::object module::globalfuncs::get_str(const std::string& name, sol::this_state ts)
{
	GET_APP;
	return app->global->get_str(name, ts);
}
void module::globalfuncs::throw_string(const std::string& msg)
{
	throw ylib::exception(msg);
}
