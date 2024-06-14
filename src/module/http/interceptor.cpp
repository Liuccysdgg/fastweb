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

#include "interceptor.h"
#include "core/app.h"

module::interceptor::interceptor()
{
}

module::interceptor::~interceptor()
{
}

bool module::interceptor::add(const std::string& pattern, const std::string& filepath, sol::this_state ts)
{
	GET_APP;
	return app->interceptor->add(pattern,filepath);
}

bool module::interceptor::remove(const std::string& pattern, sol::this_state ts)
{
	GET_APP;
	return app->interceptor->remove(pattern);
}

bool module::interceptor::exist(const std::string& pattern, sol::this_state ts)
{
	GET_APP;
	return app->interceptor->exist(pattern);
}

void module::interceptor::clear(sol::this_state ts)
{
	GET_APP;
	return app->interceptor->clear();
}

void module::interceptor::regist(sol::state* lua)
{
	lua->new_usertype<module::interceptor>("interceptor",
		"add", &module::interceptor::add,
		"remove", &module::interceptor::remove,
		"exist", &module::interceptor::exist,
		"clear", &module::interceptor::clear
	);
}
