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

#include "globalfuns.h"
#include <thread>
#include "util/counter.hpp"
#include "util/codec.h"
#include "util/time.h"
#include "core/global.h"
#include "core/app.h"
#include "core/statemanager.h"
static ylib::counter<uint64> s_counter_guid;
std::string module::globalfuncs::website_dir(sol::this_state ts)
{
	GET_APP;
	return app->config->website.dir;
}
void module::globalfuncs::create_env(const std::string& lua_filepath,sol::this_state ts)
{
	GET_APP;
	std::thread t([](std::string lua_filepath,fastweb::app* a) {
		auto lua = a->state->get();
		std::string exception_string;
		try
		{
			sol::load_result script = lua->state->load_file(a->config->website.dir + lua_filepath);
			if (!script.valid()) {
				sol::error err = script;
				throw ylib::exception(err.what());
			}
			sol::protected_function_result result = script();
			if (!result.valid()) {
				sol::error err = result;
				throw ylib::exception(err.what());
			}
		}
		catch (const std::exception& e)
		{
			if (a->config->website.debug)
				a->log->error("[create_env][" + lua_filepath + "]: " + e.what(), __FILE__, __func__, __LINE__);
		}
		lua->state->collect_garbage();
		a->state->push(lua);
	},lua_filepath,app);

	t.detach();
}
std::optional<int64> module::globalfuncs::toint(const sol::object& obj)
{
	sol::type t = obj.get_type();
	switch (t) {
	case sol::type::number:
	{
		double value = obj.as<double>();
		// 检查小数部分是否为零
		if (std::floor(value) == value) {
			return static_cast<int64>(value);
		}
		return std::nullopt;
	}
	case sol::type::string: {
		// 尝试将字符串转换为 int
		std::string s = obj.as<std::string>();
		try {
			return std::stoll(s);
		}
		catch (const std::exception&) {
			return std::nullopt;
		}
	}
	case sol::type::boolean:
		// 布尔值 true 转为 1, false 转为 0
		return obj.as<bool>() ? 1 : 0;
	default:
		// 其它类型返回空
		return std::nullopt;
	}
}
void module::globalfuncs::regist(sol::state* lua)
{
	lua->set_function("fw_set_ptr", module::globalfuncs::set_ptr);
	lua->set_function("fw_get_str", module::globalfuncs::get_str);
	lua->set_function("fw_set_str", module::globalfuncs::set_str);
	lua->set_function("fw_make_software_guid", module::globalfuncs::make_software_guid);
	lua->set_function("fw_throw_string", module::globalfuncs::throw_string);
	lua->set_function("print", module::globalfuncs::print);
	lua->set_function("fw_website_dir", module::globalfuncs::website_dir);
	lua->set_function("fw_sleep_msec",ylib::system::sleep_msec);
	lua->set_function("fw_now_msec", ylib::time::now_msec);
	lua->set_function("fw_now_sec", ylib::time::now_sec);
	lua->set_function("fw_create_env", module::globalfuncs::create_env);
	lua->set_function("fw_toint", module::globalfuncs::toint);
	

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
