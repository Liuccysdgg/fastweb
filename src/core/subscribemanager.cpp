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

#include "core/subscribemanager.h"
#include "core/config.h"
#include "core/app.h"
#include "core/statemanager.h"
#include "module/http/request.h"
#include "module/http/response.h"
#include "net/http_subscribe.h"
fastweb::subscribe_manager::subscribe_manager(fastweb::app* app) :Interface(app)
{
}
fastweb::subscribe_manager::~subscribe_manager()
{
	clear();
}
void fastweb::subscribe_manager::start()
{

	app()->router->other([&](network::http::request* request, network::http::response* response) {
		this->other(request, response);
		});
}
bool fastweb::subscribe_manager::add(const std::string& pattern, const std::string& filepath)
{
	app()->router->subscribe()->add(pattern, filepath, [&](network::http::request* request, network::http::response* response, const std::string& pattern, const std::string& filepath)->bool {
		return this->callback(request, response, pattern, filepath);
	});
	return true;
}
bool fastweb::subscribe_manager::remove(const std::string& pattern)
{
	return app()->router->subscribe()->remove(pattern);
}
bool fastweb::subscribe_manager::exist(const std::string& pattern)
{
	return app()->router->subscribe()->exist(pattern);
}
void fastweb::subscribe_manager::clear()
{
	if(app()->router != nullptr)
		app()->router->subscribe()->clear();
}

bool fastweb::subscribe_manager::callback(network::http::request* request, network::http::response* response, const std::string& pattern, const std::string& filepath)
{
	bool ok_continue = false;
	auto lua = app()->state->get();
	std::string exception_string;
	try
	{
		sol::load_result script = lua->state->load_file(app()->config->website.dir + filepath);
		if (!script.valid()) {
			sol::error err = script;
			throw ylib::exception(err.what());
		}
		module::request m_request(request);
		module::response m_response(response);
		(*lua->state)["response"] = m_response;
		(*lua->state)["request"] = m_request;

		sol::protected_function_result result = script();
		if (!result.valid()) {
			sol::error err = result;
			throw ylib::exception(err.what());
		}
	}
	catch (const std::exception& e)
	{
		exception_string = e.what();
		if (app()->config->website.debug)
			LOG_ERROR("[subscribe][" + request->filepath() + "]: " + e.what());
	}
	lua->state->collect_garbage();
	app()->state->push(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);

	return ok_continue;
}

void fastweb::subscribe_manager::other(network::http::request* request, network::http::response* response)
{
	std::string filepath = request->filepath();
	if (filepath.empty())
		filepath = "/";

	

	// 搜寻默认页面
	if (filepath[filepath.length()-1] == '/')
	{
		std::string default_filepath;
		for (size_t i = 0; i < app()->config->website.default_index.size(); i++)
		{
			default_filepath = app()->config->website.dir + filepath + app()->config->website.default_index[i];
			if (ylib::file::exist(default_filepath))
			{
				filepath += app()->config->website.default_index[i];
				break;
			}
		}
	}

	std::string ext = ylib::file::ext(filepath);
	// 判断LUA
	if (ext == "lua")
	{
		if (app()->config->website.direct_url_mapping)
		{
			callback(request,response,filepath,filepath);
		}
		else
		{
			send_404(response);
		}
		return;
	}

	// 发送静态文件
	std::string absolute_path = app()->config->website.dir + filepath;
	if (ylib::file::exist(absolute_path))
		response->send_file(absolute_path);
	else
		send_404(response);
}

void fastweb::subscribe_manager::send_404(network::http::response* response)
{
	std::string default_404 = app()->config->website.dir + "\\" + app()->config->website.default_404;
	if (app()->config->website.default_404 == "" || ylib::file::exist(default_404) == false)
	{
		response->send((std::string)"404 Not Found", 404, "Not Found");
	}
	else
	{
		response->send_file(default_404, -1, 404, "Not Found");
	}
}
