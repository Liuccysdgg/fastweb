/*Software License

Copyright(C) 2024[liuyingjie]
License Terms
Usage Rights

Any individual or entity is free to use, copy, and distribute the binary form of this software without modification to the source code, without the need to disclose the source code.
If the source code is modified, the modifications must be open - sourced under the same license.This means that the modifications must be disclosed and accompanied by a copy of this license.
Future Versions Updates
From this version onwards, all future releases will be governed by the terms of the latest version of the license.This license will automatically be nullified and replaced by the new version.
Users must comply with the terms of the new license issued in future releases.
Liability and Disclaimer
This software is provided “as is”, without any express or implied warranties, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non - infringement.In no event shall the author or copyright holder be liable for any claims, damages, or other liabilities, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
Contact Information
If you have any questions, please contact us: 1585346868@qq.com Or visit our website fwlua.com.
*/

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
