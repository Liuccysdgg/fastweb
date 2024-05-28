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
#include "core/statemanager.h"

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
	network::http::host_config host_config;
	host_config.domain = "0.0.0.0";
	host_config.port =(ushort)sConfig->server.port;

	host_config.ssl = false;

	ws_config.name = "master";
	ws_config.host.push_back(host_config);
	ws_config.router.threadpool.size = 40;
	ws_config.router.threadpool.queuemax = 10000;
	ws_config.session.dirpath = sConfig->website.session_dir;
	ws_config.session.timeout_sec = sConfig->website.session_timeout_sec;
	config.website.push_back(ws_config);
	if (!m_center->create(config))
	{
		m_lastErrorDesc = "start failed," + m_center->last_error();
		stop();
		return false;
	}
	auto website = m_center->website(sConfig->server.address);
	auto router = website->router();
	
	// 初始化脚本
	if (initialization_script() == false)
		return false;
	// 加载服务脚本
	{
		auto luas = ylib::file::traverse(sConfig->scripts.app_dir, "(.*\\.lua)");
		for_iter(iter, luas) {
			if (iter->second == IS_DIRECTORY)
				continue;
			std::string path = strutils::replace(iter->first, '\\', '/');
			if (service_bytecode.create(path, sConfig->scripts.app_dir + "/" + path, true) == false)
			{
				m_lastErrorDesc = service_bytecode.last_error();
				return false;
			}
		}
	}
	// 加载拦截器脚本
	for_iter(iter, sConfig->website.interceptor_scripts) {
		if (interceptor_bytecode.create(iter->regex_express, iter->filepath, true) == false)
		{
			m_lastErrorDesc = interceptor_bytecode.last_error();
			return false;
		}
	}
	// 加入LUA服务映射
	{
		auto map = service_bytecode.map();
		for_iter(iter, map)
		{
			auto state = sStateMgr->get_state();
			std::string route_pattern;
			network::http::method method = network::http::ALL;
			try
			{
				auto result = state->script_file(iter->second->filepath);
				if (result.valid()) {
					auto router = (*state)["route"];
					auto type = router.get_type();
					if (router.is<sol::table>())
					{
						sol::optional<std::string> route_pattern_param = router[1];
						sol::optional<int> method_param = router[2];
						if (route_pattern_param && route_pattern_param->empty() == false)
							route_pattern = *route_pattern_param;
						if (method_param)
							method = (network::http::method)*method_param;
					}
				}
			}
			catch (const std::exception& e)
			{
				LOG_ERROR(e.what());
			}
			if (route_pattern.empty())
				route_pattern = sConfig->scripts.app_mapping_dir + iter->first;

			// OutPutLog
			{
				std::string log;
				log = "[subscribe] lua: " + iter->first + "\t pattern: " + route_pattern + "\t method: ";
				switch (method)
				{
				case ylib::network::http::GET:
					log.append("GET");
					break;
				case ylib::network::http::POST:
					log.append("POST");
					break;
				case ylib::network::http::PUT:
					log.append("PUT");
					break;
				case ylib::network::http::DEL:
					log.append("DEL");
					break;
				case ylib::network::http::HEAD:
					log.append("HEAD");
					break;
				case ylib::network::http::ALL:
					log.append("ALL");
					break;
				default:
					break;
				}
				LOG_INFO(log);
			}
			router->subscribe(route_pattern, method, &fastweb::subscribe_service,new std::string(iter->first));
		}
			
	}
	
	// 加入拦截器
	for(size_t i=0;i<sConfig->website.interceptor_scripts.size();i++)
		router->interceptor()->add(sConfig->website.interceptor_scripts[i].regex_express,&fastweb::subscribe_interceptor);


	router->other([&](network::http::request* request, network::http::response* response) {
		if (request->filepath() == "/")
		{
			bool find = false;
			for (size_t i = 0; i < sConfig->website.default_index.size(); i++)
			{
				std::string filepath = sConfig->website.static_dir + request->filepath() + sConfig->website.default_index[i];
				if (ylib::file::exist(filepath))
				{
					find = true;
					response->send_file(filepath);
					break;
				}
			}
			if (find == false)
				send_404(response);
			return;
		}

		send_file(response,request->filepath());
	});


	return m_center->start();
}

void fastweb::stop()
{
	if (m_center != nullptr)
	{
		delete m_center;
	}
	m_center = nullptr;
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
	auto state = sStateMgr->get_state();
	try
	{
		state->set_function("global_regist", module::global_regist);
		auto result = state->script_file(script_filepath);
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
	return m_lastErrorDesc == "";
}

void fastweb::subscribe_service(network::http::request* request, network::http::response* response,void *extra)
{
	std::string lua_name = *(std::string*)extra;

	// 文件原路径(非绝对路径)
	//std::string lua_name = strutils::right(request->filepath(), request->filepath().length() - sConfig->scripts.app_mapping_dir.length());


	auto lua = sStateMgr->get_state();
	std::string exception_string;
	try
	{
		auto bytecode = sFastWeb->service_bytecode.get(lua_name);
		if (bytecode.empty())
			throw ylib::exception("Serious error: Bytecode not found, possibly due to pre compilation modification error. Please recheck the script file, "+lua_name);
		auto lbResult = lua->load_buffer(bytecode.data(), bytecode.length(), "bytecode");
		if (lbResult.valid() == false)
		{
			sol::error err = lbResult;
			throw ylib::exception("Failed to load bytecode, " + std::string(err.what()));
		}
		module::request m_request(request);
		module::response m_response(response);
		
		lbResult();

		(*lua)["response"] = &m_response;
		(*lua)["request"] = &m_request;

		auto result = (*lua)["access"]();
		if (!result.valid()) {
			sol::error err = result;
			throw ylib::exception(err.what());
		}
	}
	catch (const std::exception& e)
	{
		exception_string = e.what();
		if(sConfig->website.debug)
			LOG_ERROR("[subscribe_service]["+ request->filepath() + "]: "+e.what());
	}
	lua->collect_garbage();
	sStateMgr->push_state(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);
}

bool fastweb::subscribe_interceptor(network::http::reqpack* reqpack, const std::string& express_string)
{
	
	bool ok_continue = false;
	auto lua = sStateMgr->get_state();
	std::string exception_string;
	try
	{
		const std::string& bytecode = sFastWeb->interceptor_bytecode.get(express_string);
		if (bytecode.empty())
			throw ylib::exception("[interceptor] Serious error: Bytecode not found, possibly due to pre compilation modification error. Please recheck the script file, " + express_string);

		auto lbResult = lua->load_buffer(bytecode.data(), bytecode.length(), "bytecode");
		if (lbResult.valid() == false)
		{
			sol::error err = lbResult;
			throw ylib::exception("[interceptor] Failed to load bytecode, " + std::string(err.what()));
		}
		module::request m_request(reqpack->request());
		module::response m_response(reqpack->response());

		lbResult();

		(*lua)["response"] = m_response;
		(*lua)["request"] = m_request;

		auto result = (*lua)["access"]();
		if (!result.valid()) {
			sol::error err = result;
			throw ylib::exception(err.what());
		}
		ok_continue = result.get<bool>();
	}
	catch (const std::exception& e)
	{
		exception_string = e.what();
		if (sConfig->website.debug)
			LOG_ERROR("[subscribe_interceptor][" + reqpack->request()->filepath() + "]: " + e.what());
	}
	sStateMgr->push_state(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);

	return ok_continue;
}

void fastweb::send_file(network::http::response* response, std::string filepath)
{
	filepath = sConfig->website.static_dir + filepath;
	if (ylib::file::exist(filepath))
	{
		response->send_file(filepath);
	}
	else
	{
		send_404(response);
	}
}

void fastweb::send_404(network::http::response* response)
{
	std::string default_404 = sConfig->website.static_dir + "\\" + sConfig->website.default_404;
	if (sConfig->website.default_404 == "" || ylib::file::exist(default_404) == false)
	{
		response->send((std::string)"404 Not Found",404,"Not Found");
	}
	else
	{
		response->send_file(default_404,-1, 404, "Not Found");
	}
}
