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
// Mobile：17367918735
// QQ：1585346868

#include "globalfuns.h"
#include "util/counter.hpp"
#include "util/codec.h"
#include "util/time.h"
#include "core/global.h"
#include "core/app.h"
static ylib::counter<uint64> s_counter_guid;
std::string module::globalfuncs::website_dir(sol::this_state ts)
{
	GET_APP;
	return app->config->website.dir;
}
void module::globalfuncs::regist(sol::state* lua)
{
	lua->set_function("set_ptr", module::globalfuncs::set_ptr);
	lua->set_function("get_str", module::globalfuncs::get_str);
	lua->set_function("set_str", module::globalfuncs::set_str);
	lua->set_function("make_software_guid", module::globalfuncs::make_software_guid);
	lua->set_function("throw_string", module::globalfuncs::throw_string);
	lua->set_function("print", module::globalfuncs::print);
	lua->set_function("website_dir", module::globalfuncs::website_dir);

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
