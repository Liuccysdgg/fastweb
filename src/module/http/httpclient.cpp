// MIT License﻿
// Copyright(c) 2024 FastWeb - fwlua.com - nianhua﻿
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :﻿
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.﻿
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.﻿
// ## Additional Terms for Commercial Use﻿
// This software is licensed for personal, educational, and non - commercial use.
// For commercial use or use within a company, organization, or institution, a
// separate commercial license is required.To obtain a commercial license,
// please contact
// EMail：1585346868@qq.com

// QQ：1585346868

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
