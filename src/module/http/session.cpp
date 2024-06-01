#include "session.h"
#include "net/http_website.h"
module::session::session(network::http::session* session):m_session(session)
{
}

module::session::~session()
{
}

void module::session::init(module::request& request, const std::string& id)
{
	m_session->init((network::http::website*)request.website(),id);
}

std::string module::session::id()
{
	return m_session->id();
}

void module::session::update()
{
	m_session->update();
}

void module::session::set(const std::string& name, const std::string& value)
{
	m_session->set(name, value);
}

std::string module::session::get(const std::string& name)
{
	return m_session->get(name);
}

bool module::session::check()
{
	return m_session->check();
}

void module::session::regist(sol::state* lua)
{
	lua->new_usertype<module::session>("module_session",
		"check", &module::session::check,
		"get", &module::session::get,
		"id", &module::session::id,
		"set", &module::session::set,
		"update", &module::session::update,
		"init", &module::session::init
	);
}
