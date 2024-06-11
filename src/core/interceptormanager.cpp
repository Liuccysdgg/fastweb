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

#include "core/interceptormanager.h"
#include "core/config.h"
#include "core/app.h"
#include "core/statemanager.h"
#include "module/http/request.h"
#include "module/http/response.h"
#include "net/http_interceptor.h"
fastweb::interceptor_manager::interceptor_manager(fastweb::app* app):Interface(app)
{
}
fastweb::interceptor_manager::~interceptor_manager()
{
	clear();
}
bool fastweb::interceptor_manager::add(const std::string& pattern, const std::string& filepath)
{
	app()->router->interceptor()->add(pattern,filepath, [&](network::http::reqpack* reqpack, const std::string& pattern,const std::string& filepath)->bool {
		return this->callback(reqpack, pattern, filepath);
	});
	return true;
}
bool fastweb::interceptor_manager::remove(const std::string& pattern)
{
	return app()->router->interceptor()->remove(pattern);
}
bool fastweb::interceptor_manager::exist(const std::string& pattern)
{
	return app()->router->interceptor()->exist(pattern);
}
void fastweb::interceptor_manager::clear()
{
	if(app()->router != nullptr)
		app()->router->interceptor()->clear();
}

bool fastweb::interceptor_manager::callback(network::http::reqpack* reqpack, const std::string& pattern, const std::string& filepath)
{
	if (ylib::file::ext(filepath) != "lua")
	{
		reqpack->response()->send_file(filepath);
		return false;
	}
	bool ok_continue = false;
	auto lua = app()->state->get();
	std::string exception_string;
	try
	{
		sol::load_result script = lua->state->load_file(app()->config->website.dir+ filepath);
		if (!script.valid()) {
			sol::error err = script;
			throw ylib::exception(err.what());
		}
		module::request m_request(reqpack->request());
		module::response m_response(reqpack->response());
		(*lua->state)["response"] = m_response;
		(*lua->state)["request"] = m_request;

		sol::protected_function_result result = script();
		if (!result.valid()) {
			sol::error err = result;
			throw ylib::exception(err.what());
		}
		ok_continue = result.get<bool>();
	}
	catch (const std::exception& e)
	{
		exception_string = e.what();
		if (app()->config->website.debug)
			LOG_ERROR("[interceptor][" + reqpack->request()->filepath() + "]: " + e.what());
	}
	lua->state->collect_garbage();
	app()->state->push(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);

	return ok_continue;
}
