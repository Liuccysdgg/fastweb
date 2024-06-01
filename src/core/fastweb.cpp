#include "fastweb.h"
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
#include "core/statemanager.h"
#include "core/subscribemanager.h"
#include "core/interceptormanager.h"
bool fastweb::start()
{

	stop();
	// 虚拟机管理器
	if(sStateMgr->start() == false)
	{
		m_lastErrorDesc = sStateMgr->last_error();
		return false;
	}

	m_center = new network::http::center();
	network::http::start_config config;
	network::http::website_config ws_config;
	
	for_iter(iter, sConfig->domain)
	{
		network::http::host_config host_config;
		host_config.domain = iter->first;
		host_config.port = iter->second.port;
		host_config.ssl = iter->second.https;
		ws_config.host.push_back(host_config);
	}

	ws_config.name = "master";
	
	ws_config.router.threadpool.size = 40;
	ws_config.router.threadpool.queuemax = 10000;
	ws_config.session.dirpath = sConfig->website.session_dir;
	ws_config.session.timeout_sec = sConfig->website.session_timeout_sec;
	config.website.push_back(ws_config);
	for_iter(iter, sConfig->domain)
	{
		if(iter->second.https) 
			config.cert.emplace(iter->first, iter->second.ssl);
	}

	if (!m_center->create(config))
	{
		m_lastErrorDesc = "start failed," + m_center->last_error();
		stop();
		return false;
	}
	auto website = m_center->website_byname(ws_config.name);
	auto router = website->router();
	
	// 初始化脚本
	if (initialization_script() == false)
		return false;


	// 加载订阅
	m_subscribe.load(router);
	// 加载拦截器
	m_interceptor.load(router);
	
		

	return m_center->start();
}

void fastweb::stop()
{
	m_subscribe.clear();
	m_interceptor.clear();
	if (m_center != nullptr)
	{
		m_center->close();
		delete m_center;
	}
	m_center = nullptr;

	global::getInstance()->clear();
	if (m_state_init != nullptr)
		delete m_state_init;
	m_state_init = nullptr;
	sStateMgr->close();
}

bool fastweb::initialization_script()
{
	auto script_filepath = sConfig->website.Initialization_script;
	if (script_filepath == "")
		return true;
	if (ylib::file::exist(script_filepath) == false)
	{
		m_lastErrorDesc = "Initialization script not found, filepath: " + script_filepath;
		return false;
	}
	auto state = sStateMgr->get();
	try
	{
		state->state->set_function("global_regist", module::global_regist);
		auto result = state->state->script_file(script_filepath);
		if (!result.valid()) {
			sol::error err = result;
			throw ylib::exception(err.what());
		}
		if (result.get<bool>() == false)
		{
			throw ylib::exception("user interrupt");
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