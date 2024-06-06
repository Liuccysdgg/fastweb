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
bool fastweb::interceptor_manager::add(const std::string& regex_express, const std::string& filepath)
{
	if (interceptor_manager::interceptor.exist(regex_express))
	{
		LOG_WARN("`" + regex_express + "` interceptor is invalid, duplicate rule, filepath: " + filepath);
		return false;
	}
	interceptor_manager::interceptor.add(regex_express, filepath);
	app()->router->interceptor()->add(regex_express, [this](network::http::reqpack* reqpack, const std::string& express_string)->bool {
		return this->callback(reqpack, express_string);
	});
	return true;
}
bool fastweb::interceptor_manager::remove(const std::string& regex_express)
{
	interceptor_manager::interceptor.del(regex_express);
	return app()->router->interceptor()->remove(regex_express);
}
bool fastweb::interceptor_manager::exist(const std::string& regex_express)
{
	return interceptor_manager::interceptor.exist(regex_express);
}
void fastweb::interceptor_manager::clear()
{
	interceptor_manager::interceptor.clear();
	if(app()->router != nullptr)
		app()->router->interceptor()->clear();
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
			LOG_ERROR("[interceptor][" + reqpack->request()->filepath() + "]: " + e.what());
	}
	lua->state->collect_garbage();
	app()->state->push(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);

	return ok_continue;
}
