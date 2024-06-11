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
// Mobile：17367918735
// QQ：1585346868

#include "subscribe.h"
#include "core/app.h"

module::subscribe::subscribe()
{
}

module::subscribe::~subscribe()
{
}

bool module::subscribe::add(const std::string& express_string, const std::string& filepath, sol::this_state ts)
{
	GET_APP;
	return app->subscribe->add(express_string,filepath);
}

bool module::subscribe::remove(const std::string& express_string, sol::this_state ts)
{
	GET_APP;
	return app->subscribe->remove(express_string);
}

bool module::subscribe::exist(const std::string& express_string, sol::this_state ts)
{
	GET_APP;
	return app->subscribe->exist(express_string);
}

void module::subscribe::clear(sol::this_state ts)
{
	GET_APP;
	return app->subscribe->clear();
}

void module::subscribe::regist(sol::state* lua)
{
	lua->new_usertype<module::subscribe>("subscribe",
		"add", &module::subscribe::add,
		"remove", &module::subscribe::remove,
		"exist", &module::subscribe::exist,
		"clear", &module::subscribe::clear
	);
}
