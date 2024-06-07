#include "subscribemanager.h"
#include "core/config.h"
#include "core/app.h"
#include "core/statemanager.h"
#include "module/http/request.h"
#include "module/http/response.h"
fastweb::subscribe_manager::subscribe_manager(fastweb::app* ptr):Interface(ptr)
{
}
fastweb::subscribe_manager::~subscribe_manager()
{
	clear();
}
void fastweb::subscribe_manager::load(network::http::router* router)
{
	clear();
	m_router = router;
	router->other([&](network::http::request* request, network::http::response* response) {
		this->other(request,response);
	});
}

void fastweb::subscribe_manager::clear()
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
void fastweb::subscribe_manager::other(network::http::request* request, network::http::response* response)
{
	auto send_404 = [&](network::http::response* response) {
		std::string default_404 = app()->config->website.dir + "\\" + app()->config->website.default_404;
		if (app()->config->website.default_404 == "" || ylib::file::exist(default_404) == false)
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
	if (strutils::right(request->filepath(),1) == "/")
	{
		for (size_t i = 0; i < app()->config->website.default_index.size(); i++)
		{
			filepath = app()->config->website.dir + request->filepath() + app()->config->website.default_index[i];
			if (ylib::file::exist(filepath))
				break;
		}
	}
	else
		filepath = app()->config->website.dir + request->filepath();
	send_file(response, filepath);
}

void fastweb::subscribe_manager::exec(const std::string& filepath, network::http::request* request, network::http::response* response)
{

	auto lua = app()->state->get();
	std::string exception_string;
	try
	{
		//auto lbResult = lua->state->load_file(filepath);
		//if (lbResult.valid() == false)
		//{
		//	sol::error err = lbResult;
		//	throw ylib::exception("Failed to load script, " + std::string(err.what()));
		//}
		//module::request m_request(request);
		//module::response m_response(response);

		//(*lua->state)["response"] = &m_response;
		//(*lua->state)["request"] = &m_request;
		//lbResult();

		sol::load_result script = lua->state->load_file(filepath);
		if (!script.valid()) {
			sol::error err = script;
			throw ylib::exception(err.what());
		}
		module::request m_request(request);
		module::response m_response(response);
		(*lua->state)["response"] = &m_response;
		(*lua->state)["request"] = &m_request;

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
			LOG_ERROR("[subscribe_service][" + request->filepath() + "]: " + e.what());
	}
	// 清理
	lua->state->collect_garbage();
	app()->state->push(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);
}
