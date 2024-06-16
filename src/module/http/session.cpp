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
	if (&request == nullptr)
	{
		throw ylib::exception("the `request` parameter of the init function is nil");
	}
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
	lua->new_usertype<module::session>("fw_session",
		"check", &module::session::check,
		"get", &module::session::get,
		"id", &module::session::id,
		"set", &module::session::set,
		"update", &module::session::update,
		"init", &module::session::init
	);
}
