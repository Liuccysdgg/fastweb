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
void fastweb::interceptor_manager::load(network::http::router* router)
{
	clear();
	m_router = router;
	for (size_t i = 0; i < app()->config->website.interceptor_scripts.size(); i++)
	{
		interceptor_manager::interceptor.emplace(app()->config->website.interceptor_scripts[i].regex_express, app()->config->website.interceptor_scripts[i].filepath);
		router->interceptor()->add(app()->config->website.interceptor_scripts[i].regex_express, [&](network::http::reqpack* reqpack, const std::string& express_string)->bool {
			return this->callback(reqpack, express_string);
		});
	}
}

void fastweb::interceptor_manager::clear()
{
	if(m_router != nullptr)
		m_router->interceptor()->clear();
	interceptor_manager::interceptor.clear();
	m_router = nullptr;
}

bool fastweb::interceptor_manager::callback(network::http::reqpack* reqpack, const std::string& express_string)
{
	bool ok_continue = false;
	auto lua = app()->state->get();
	std::string exception_string;
	try
	{
		sol::load_result script = lua->state->load_file(interceptor_manager::interceptor[express_string]);
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
			LOG_ERROR("[subscribe_interceptor][" + reqpack->request()->filepath() + "]: " + e.what());
	}
	lua->state->collect_garbage();
	app()->state->push(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);

	return ok_continue;
}
