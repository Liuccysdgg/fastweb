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

#include "app.h"
#include <regex>
#include "net/http_website.h"
#include "net/http_router.h"
#include "net/http_interceptor.h"
#include "net/http_reqpack.h"
#include "module/http/request.h"
#include "module/http/response.h"
#include "module/globalfuns.h"
#include "core/config.h"
#include "core/global.h"
fastweb::app::app()
{
	log = std::make_shared<fastweb::log>(this);
	subscribe = std::make_shared<fastweb::subscribe_manager>(this);
	interceptor = std::make_shared<fastweb::interceptor_manager>(this);
	state = std::make_shared<fastweb::state_manager>(this);
	config = std::make_shared<fastweb::config>(this);
	global = std::make_shared<fastweb::global>(this);
	gloabl_module = std::make_shared<fastweb::global_module>(this);
}
fastweb::app::~app()
{
	stop();
}
bool fastweb::app::start(const std::string& config_filepath)
{

	stop();
	// 初始化配置
	if (config->open(config_filepath) == false)
	{
		m_lastErrorDesc = "open config failed, " + config->last_error();
		return false;
	}
	// 虚拟机管理器
	if(state->start() == false)
	{
		m_lastErrorDesc = state->last_error();
		return false;
	}

	m_center = new network::http::center();
	network::http::start_config config;
	network::http::website_config ws_config;
	
	for_iter(iter, this->config->domain)
	{
		network::http::host_config host_config;
		host_config.domain = iter->first;
		host_config.port = iter->second.port;
		host_config.ssl = iter->second.https;
		ws_config.host.push_back(host_config);

		std::string url;
		if (host_config.ssl)
			url = "https://";
		else
			url = "http://";
		url += host_config.domain;
		url += ":" + std::to_string(host_config.port);
		log->info("URL: " + url, __FILE__, __func__, __LINE__);
	}

	ws_config.name = "master";
	
	ws_config.router.threadpool.size = 40;
	ws_config.router.threadpool.queuemax = 10000;
	ws_config.session.dirpath = this->config->website.session_dir;
	ws_config.session.timeout_sec = this->config->website.session_timeout_sec;
	config.website.push_back(ws_config);
	for_iter(iter, this->config->domain)
	{
		if(iter->second.https) 
			config.cert.emplace(iter->first, iter->second.ssl);
	}

	if (!m_center->create(config))
	{
		m_lastErrorDesc = "start failed," + m_center->last_error();
		return false;
	}
	auto website = m_center->website_byname(ws_config.name);
	router = website->router();
	
	// 初始化脚本
	if (initialization_script() == false)
		return false;
		
	this->subscribe->start();

	if (m_center->start() == false)
	{
		m_lastErrorDesc = m_center->last_error();
		return false;
	}
	return true;
}

void fastweb::app::stop()
{
	this->m_lastErrorDesc.clear();
	
	subscribe->clear();
	interceptor->clear();
	router = nullptr;
	if (m_center != nullptr)
	{
		m_center->close();
		delete m_center;
	}
	m_center = nullptr;

	global->clear();
	if (m_state_init != nullptr)
		delete m_state_init;
	m_state_init = nullptr;
	state->close();
}

bool fastweb::app::initialization_script()
{
	auto script_filepath = this->config->website.Initialization_script;
	if (script_filepath == "")
		return true;
	if (script_filepath[0] != '/')
		script_filepath = "/" + script_filepath;
	script_filepath = config->website.dir + script_filepath;
	if (ylib::file::exist(script_filepath) == false)
	{
		m_lastErrorDesc = "Initialization script not found, filepath: " + script_filepath;
		return false;
	}
	auto state = this->state->get();
	try
	{
		sol::load_result script = state->state->load_file(script_filepath);
		if (!script.valid()) {
			sol::error err = script;
			throw ylib::exception(err.what());
		}

		sol::protected_function_result result = script();
		if (!result.valid()) {
			sol::error err = result;
			throw ylib::exception(err.what());
		}
		if (result.get<bool>() == false)
		{
			throw ylib::exception("user interrupt: `return false`");
		}
	}
	catch (const std::exception& e)
	{
		m_lastErrorDesc = "Exception in executing initialization script: " + std::string(e.what());
	}
	// 不可DELETE，否则注册的全局变量会被自动销毁
	//delete state;
	m_state_init = state;
	return m_lastErrorDesc == "";
}