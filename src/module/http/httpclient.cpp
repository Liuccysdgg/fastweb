#include "httpclient.h"

module::httpclient::httpclient()
{
}

module::httpclient::~httpclient()
{
}

void module::httpclient::set_timeout(uint32 connect_msec, uint32 recv_msec)
{
	m_client.set_timeout(connect_msec,recv_msec);
}

bool module::httpclient::get(const std::string& url)
{
	return m_client.get(url);
}

bool module::httpclient::post(const std::string& url, const std::string& body)
{
	return m_client.post(url,ylib::buffer(body));
}

void module::httpclient::request_header(const std::string& name, const std::string& value)
{
	m_client.headers_request().set(name, value);
}

std::string module::httpclient::response_header(const std::string& name)
{
	return m_client.headers_response().get(name).to_string();
}

std::string module::httpclient::response()
{
	return m_client.response().to_string();
}

ushort module::httpclient::status()
{
	return m_client.status();
}

void module::httpclient::regist(sol::state* lua)
{
	lua->new_usertype<module::httpclient>("httpclient",
		"new", sol::constructors<module::httpclient()>(),
		"get", &module::httpclient::get,
		"post", &module::httpclient::post,
		"set_timeout", &module::httpclient::set_timeout,
		"request_header", &module::httpclient::request_header,
		"response_header", &module::httpclient::response_header,
		"status", &module::httpclient::status,
		"response", &module::httpclient::response
	);
}
