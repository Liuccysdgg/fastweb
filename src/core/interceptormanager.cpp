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
