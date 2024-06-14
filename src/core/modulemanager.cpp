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

#include "modulemanager.h"

#include "util/file.h"
#include "core/app.h"
#include "core/config.h"
#include "core/global.h"
#ifdef _WIN32
#include <Windows.h>
#else

#endif

#include "module/http/request.h"
#include "module/http/response.h"
#include "module/http/session.h"
#include "module/http/httpclient.h"
#include "module/http/interceptor.h"
#include "module/http/subscribe.h"
#include "module/globalfuns.h"
#include "module/mutex.h"
#include "module/codec.h"
#include "module/time.h"
#include "module/filesystem.h"
#include "module/sys.h"
#include "module/timer.h"
#include "module/process.h"
#include "module/ini.h"
fastweb::module_manager::module_manager(fastweb::app* app):Interface(app)
{
}
fastweb::module_manager::~module_manager()
{
}
void fastweb::module_manager::start()
{
#ifdef _WIN32
	auto get_last_error_desc = []()->std::string {
		DWORD dwError = GetLastError();
		// 获取错误描述  
		char* lpMsgBuf = nullptr;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);
		// 输出错误描述  
		return std::string(lpMsgBuf);
	};
#endif
	close();
	auto ms = modules();
	for (size_t i = 0; i < ms.size(); i++)
	{
		module_info mi;
		std::string mod_filepath = app()->config->scripts.module_dir + "/" + ms[i];
#ifdef _WIN32
		SetDllDirectoryA(ylib::file::parent_dir(mod_filepath).c_str());
		mi.dll = LoadLibrary(mod_filepath.c_str());
		if (mi.dll == nullptr)
		{
			LOG_ERROR("module loading failed, filename: " + mod_filepath
				+","+ codec::to_utf8(get_last_error_desc())
			);
			continue;
		}
		mi.func = (fastweb_module_regist)GetProcAddress((HMODULE)mi.dll, "fastweb_module_regist");
		if (mi.func == nullptr) {
			LOG_ERROR("function not found: `fastweb_module_regist`, filename: " + mod_filepath);
			FreeLibrary((HMODULE)mi.dll);
			continue;
		}
		m_modules.emplace(mod_filepath, mi);
		/*if (api_func(lua, lua->lua_state()) == 0)
		{
			LOG_INFO("successfully regist module, filename: " + mod_filepath);
			continue;
		}
		LOG_ERROR("regist module failed, filename: " + mod_filepath);*/
#else

#endif
	}
}

void fastweb::module_manager::close()
{
	for_iter(iter, m_modules)
	{
#ifdef _WIN32
		FreeLibrary((HMODULE)iter->second.dll);
#else
#endif
	}
	m_modules.clear();
}

void fastweb::module_manager::load(sol::state* lua)
{
	load_core(lua);
	load_lualib(lua);
	load_3rdparty(lua);
}
void fastweb::module_manager::load_core(sol::state* lua)
{
	lua->open_libraries(
		sol::lib::base,
		sol::lib::package,
		sol::lib::math,
		sol::lib::string,
		sol::lib::table,
		sol::lib::utf8,
		sol::lib::bit32,
		sol::lib::coroutine,
		sol::lib::count,
		sol::lib::ffi,
		sol::lib::io,
		sol::lib::jit,
		sol::lib::os
	);


	module::request::regist(lua);
	module::response::regist(lua);
	module::interceptor::regist(lua);
	module::subscribe::regist(lua);
	module::session::regist(lua);
	module::httpclient::regist(lua);
	module::globalfuncs::regist(lua);
	module::mutex::regist(lua);
	module::auto_lock::regist(lua);
	module::codec::regist(lua);
	module::time::regist(lua);
	module::filesystem::regist(lua);
	module::sys::regist(lua);
	module::timer::regist(lua);
	module::ini::regist(lua);
	module::process::regist(lua);

	app()->global->regist(lua);

}

void fastweb::module_manager::load_3rdparty(sol::state* lua)
{
	for_iter(iter, m_modules)
	{ 
		if (iter->second.func(lua, lua->lua_state()) != 0)
		{
			LOG_ERROR("egist module failed, filename: "+iter->first);
		}
	}
}

void fastweb::module_manager::load_lualib(sol::state* lua)
{
	// 获取当前的package.path，添加新的搜索路径
	std::string current_path = (*lua)["package"]["path"];  // 获取当前的路径
	//for (size_t i = 0; i < app()->config->scripts.lib_dir.size(); i++)
	//	current_path += ";" + app()->config->scripts.lib_dir[i] + "/?.lua";  // 添加新的路径
	current_path += ";" + app()->config->website.dir + "/?.lua";  // 添加新的路径
	(*lua)["package"]["path"] = current_path;  // 设置修改后的路径
}

std::vector<std::string> fastweb::module_manager::modules()
{
	std::vector<std::string> results;
	auto luas = ylib::file::traverse(app()->config->scripts.module_dir, "(.*\\.dll)");
	for_iter(iter, luas)
	{
		if (iter->second == IS_DIRECTORY)
			continue;
		std::string path = strutils::replace(iter->first, '\\', '/');
		if (path.find("/") != -1)
		{
			if (strutils::split(path, '/').size() != 2)
				continue;	//多级不支持
			if (ylib::file::parent_dir(path) != ylib::file::filename(path, false))
				continue;
		}
		results.push_back(path);
	}
	return results;
}