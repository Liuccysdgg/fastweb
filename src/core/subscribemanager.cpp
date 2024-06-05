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
void subscribe_manager::other(network::http::request* request, network::http::response* response)
{
	auto send_404 = [](network::http::response* response) {
		std::string default_404 = sConfig->website.dir + "\\" + sConfig->website.default_404;
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
		if (ylib::file::exist(filepath))
		{
			if (ylib::file::ext(filepath) == "lua")
			{
				exec(filepath,request,response);
				return;
			}
			response->send_file(filepath);
		}
		else
		{
			send_404(response);
		}
	};

	std::string filepath;
	if (request->filepath() == "/")
	{
		for (size_t i = 0; i < sConfig->website.default_index.size(); i++)
		{
			filepath = sConfig->website.dir + request->filepath() + sConfig->website.default_index[i];
			if (ylib::file::exist(filepath))
				break;
		}
	}
	else
		filepath = sConfig->website.dir + request->filepath();
	send_file(response, filepath);
}

void subscribe_manager::exec(const std::string& filepath, network::http::request* request, network::http::response* response)
{

	auto lua = sStateMgr->get();
	std::string exception_string;
	try
	{
		auto lbResult = lua->state->load_file(filepath);
		if (lbResult.valid() == false)
		{
			sol::error err = lbResult;
			throw ylib::exception("Failed to load script, " + std::string(err.what()));
		}
		module::request m_request(request);
		module::response m_response(response);

		(*lua->state)["response"] = &m_response;
		(*lua->state)["request"] = &m_request;
		lbResult();
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
