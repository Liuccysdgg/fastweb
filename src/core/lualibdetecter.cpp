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

#include "lualibdetecter.h"
#include "core/config.h"
#include "core/app.h"
#include "util/file.h"
fastweb::lualib_detecter::lualib_detecter(fastweb::app* app):Interface(app)
{
}

fastweb::lualib_detecter::~lualib_detecter()
{
}

bool fastweb::lualib_detecter::changed()
{
	auto lib_files = app()->config->lua_lib_files();
	bool changed = false;
	if (lib_files.size() == m_files.size())
	{
		for (size_t i = 0; i < lib_files.size(); i++)
		{
			auto iter = m_files.find(lib_files[i]);
			if (iter == m_files.end())
			{
				changed = true;
				break;
			}
			if (ylib::file::last_write_time(lib_files[i]) != iter->second)
			{
				changed = true;
				break;
			}
		}
	}
	else
		changed = true;


	if (changed == false)
		return false;

	m_files.clear();
	for (size_t i = 0; i < lib_files.size(); i++)
		m_files.emplace(lib_files[i], ylib::file::last_write_time(lib_files[i]));

	return true;
}
