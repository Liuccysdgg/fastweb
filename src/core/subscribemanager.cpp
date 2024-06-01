#include "subscribemanager.h"
#include "core/config.h"
#include "core/statemanager.h"
#include "module/http/request.h"
#include "module/http/response.h"
subscribe_manager::subscribe_manager()
{
}
subscribe_manager::~subscribe_manager()
{
	clear();
}
void subscribe_manager::load(network::http::router* router)
{
	clear();
	m_router = router;
	// 初始化全部订阅
	auto files = sConfig->lua_app_files();
	for (size_t i = 0; i < files.size(); i++)
	{
		init_subscribe(files[i]);
	}
	// 其它绑定
	router->other(&subscribe_manager::other);
}

void subscribe_manager::clear()
{
	if (m_router != nullptr)
	{
		m_router->clear_subscribe();
	}
	for (size_t i = 0; i < m_subextra.size(); i++)
		delete m_subextra[i];
	m_subextra.clear();
	m_router = nullptr;
}

void subscribe_manager::init_subscribe(const std::string& filepath)
{
	auto state = sStateMgr->get();
	std::string route_pattern;
	network::http::method method = network::http::ALL;
	try
	{
		auto result = state->state->script_file(sConfig->scripts.app_dir + "/" + filepath);
		if (result.valid()) {
			auto router = (*state->state)["route"];
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

	sStateMgr->push(state);
	if (route_pattern.empty())
		route_pattern = sConfig->scripts.app_mapping_dir + filepath;

	// OutPutLog
	{
		std::string log;
		log = "[subscribe] lua: " + filepath + "\t pattern: " + route_pattern + "\t method: ";
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


	std::string* extra = new std::string(sConfig->scripts.app_dir + "/" + filepath);
	m_subextra.push_back(extra);
	m_router->subscribe(route_pattern, method, &subscribe_manager::callback, extra);
}

void subscribe_manager::callback(network::http::request* request, network::http::response* response, void* extra)
{
	std::string lua_filepath = *(std::string*)extra;

	auto lua = sStateMgr->get();
	std::string exception_string;
	try
	{
		auto lbResult = lua->state->load_file(lua_filepath);
		if (lbResult.valid() == false)
		{
			sol::error err = lbResult;
			throw ylib::exception("Failed to load bytecode, " + std::string(err.what()));
		}
		module::request m_request(request);
		module::response m_response(response);

		lbResult();

		(*lua->state)["response"] = &m_response;
		(*lua->state)["request"] = &m_request;

		auto result = (*lua->state)["access"]();
		if (!result.valid()) {
			sol::error err = result;
			throw ylib::exception(err.what());
		}
	}
	catch (const std::exception& e)
	{
		exception_string = e.what();
		if (sConfig->website.debug)
			LOG_ERROR("[subscribe_service][" + request->filepath() + "]: " + e.what());
	}
	// 清理
	lua->state->collect_garbage();
	sStateMgr->push(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);
}

void subscribe_manager::other(network::http::request* request, network::http::response* response)
{
	auto send_404 = [](network::http::response* response) {
		std::string default_404 = sConfig->website.static_dir + "\\" + sConfig->website.default_404;
		if (sConfig->website.default_404 == "" || ylib::file::exist(default_404) == false)
		{
			response->send((std::string)"404 Not Found", 404, "Not Found");
		}
		else
		{
			response->send_file(default_404, -1, 404, "Not Found");
		}
	};
	auto send_file = [&](network::http::response* response, std::string filepath)
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
	};

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
		{
			send_404(response);
		}
		return;
	}
	send_file(response, request->filepath());
}
