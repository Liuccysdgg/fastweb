#include "core/interceptormanager.h"
#include "core/config.h"
#include "core/statemanager.h"
#include "module/http/request.h"
#include "module/http/response.h"
#include "net/http_interceptor.h"
std::map<std::string, std::string> interceptor_manager::interceptor = std::map<std::string,std::string>();
interceptor_manager::interceptor_manager()
{
}
interceptor_manager::~interceptor_manager()
{
	clear();
}
void interceptor_manager::load(network::http::router* router)
{
	clear();
	m_router = router;
	for (size_t i = 0; i < sConfig->website.interceptor_scripts.size(); i++)
	{
		interceptor_manager::interceptor.emplace(sConfig->website.interceptor_scripts[i].regex_express, sConfig->website.interceptor_scripts[i].filepath);
		router->interceptor()->add(sConfig->website.interceptor_scripts[i].regex_express, &interceptor_manager::callback);
	}
}

void interceptor_manager::clear()
{
	if(m_router != nullptr)
		m_router->interceptor()->clear();
	interceptor_manager::interceptor.clear();
	m_router = nullptr;
}

bool interceptor_manager::callback(network::http::reqpack* reqpack, const std::string& express_string)
{
	bool ok_continue = false;
	auto lua = sStateMgr->get();
	std::string exception_string;
	try
	{
		auto lbResult = lua->state->load_file(interceptor_manager::interceptor[express_string]);
		if (lbResult.valid() == false)
		{
			sol::error err = lbResult;
			throw ylib::exception("[interceptor] Failed to load script, " + std::string(err.what()));
		}
		module::request m_request(reqpack->request());
		module::response m_response(reqpack->response());

		(*lua->state)["response"] = m_response;
		(*lua->state)["request"] = m_request;

		auto result = lbResult();
		ok_continue = result.get<bool>();
	}
	catch (const std::exception& e)
	{
		exception_string = e.what();
		if (sConfig->website.debug)
			LOG_ERROR("[subscribe_interceptor][" + reqpack->request()->filepath() + "]: " + e.what());
	}
	lua->state->collect_garbage();
	sStateMgr->push(lua);

	if (exception_string.empty() == false)
		throw ylib::exception(exception_string);

	return ok_continue;
}
