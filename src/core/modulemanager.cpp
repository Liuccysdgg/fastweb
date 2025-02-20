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

#include "modulemanager.h"

#include "util/file.h"
#include "core/app.h"
#include "core/config.h"
#include "core/global.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "module/http/request.h"
#include "module/http/response.h"
#include "module/http/session.h"
#include "module/http/httpclient.h"
#include "module/http/interceptor.h"
#include "module/http/subscribe.h"
#include "module/globalfuns.h"
#include "module/mutex.h"
#include "module/timer.h"
#include "module/process.h"
#include "module/ini.h"
#include "module/codec.h"
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
		std::string mod_filepath =ms[i];
#ifdef _WIN32
		SetDllDirectoryA(ylib::file::parent_dir(mod_filepath).c_str());
		mi.dll = LoadLibrary(mod_filepath.c_str());
		if (mi.dll == nullptr)
		{
			LOG_DEBUG("module loading failed, filename: " + mod_filepath
				+","+ codec::to_utf8(get_last_error_desc())
			);
			continue;
		}
		mi.func = (fastweb_module_regist)GetProcAddress((HMODULE)mi.dll, "fastweb_module_regist");
		if (mi.func == nullptr) {
			FreeLibrary((HMODULE)mi.dll);
			continue;
		}
		m_modules.emplace(mod_filepath, mi);
#else
		mi.dll = dlopen(mod_filepath.c_str(), RTLD_LAZY);
		if (mi.dll == nullptr)
		{
			LOG_DEBUG("module loading failed, filename: " + mod_filepath
				+ ", " + dlerror()
			);
			continue;
		}
		mi.func = (fastweb_module_regist)dlsym(mi.dll, "fastweb_module_regist");
		if (mi.func == nullptr) {
			dlclose(mi.dll);
			continue;
		}
		m_modules.emplace(mod_filepath, mi);
#endif
	}


	// 查找所有引用
	{
#ifdef _WIN32
#define DLL_EXT "dll"
#else
#define DLL_EXT "so"
#endif
#ifndef _WIN32
		// Linux
		{
			m_lua_include_path += ";/opt/luarocks/lib/lua/5.4/?.lua";
			m_lua_include_cpath += ";/opt/luarocks/lib/lua/5.4/?.so";
			m_lua_include_path += ";/opt/luarocks/share/lua/5.4/?.lua";
			m_lua_include_cpath += ";/opt/luarocks/share/lua/5.4/?.so";
			///opt/luarocks/lib/lua/5.4
		}
#endif
		// 用户
		{
			
			m_lua_include_path += ";" + app()->config->website.dir + "/?.lua";
			m_lua_include_path += ";" + app()->config->scripts.module_dir + "/?.lua";

			m_lua_include_cpath += ";" + app()->config->website.dir + "/?." + DLL_EXT;
			m_lua_include_cpath += ";" + app()->config->scripts.module_dir + "/?." + DLL_EXT;
		}
		// FastWeb管理器
		{
			std::string search_path = app()->config->scripts.module_dir + "/.install";

			std::map<std::string, bool> dirs;
			ylib::file::list(search_path, dirs);
			for_iter(iter, dirs)
			{
				if (iter->second == false)
					continue;
				if (iter->first == "." || iter->first == "..")
					continue;
				m_lua_include_path += ";" + search_path + "/" + iter->first + "/?.lua";
				m_lua_include_cpath += ";" + search_path + "/" + iter->first + "/?." + DLL_EXT;
			}
			
		}
	}
}

void fastweb::module_manager::close()
{
	for_iter(iter, m_modules)
	{
#ifdef _WIN32
		FreeLibrary((HMODULE)iter->second.dll);
#else
		dlclose(iter->second.dll);
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
	module::timer::regist(lua);
	module::process::regist(lua);
	module::ini::regist(lua);
	module::codec::regist(lua);

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
	std::string path = (*lua)["package"]["path"];
	std::string cpath = (*lua)["package"]["cpath"];

	(*lua)["package"]["path"] = path + m_lua_include_path;
	(*lua)["package"]["cpath"] = cpath + m_lua_include_cpath;
}

std::vector<std::string> fastweb::module_manager::modules()
{
	std::vector<std::string> results;

	auto insert_dll_path = [&](const std::string& dir) {
		#ifdef _WIN32
		auto luas = ylib::file::traverse(dir, "(.*\\.dll)");
		#else
		auto luas = ylib::file::traverse(dir, "(.*\\.so)");
		#endif
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
			results.push_back(dir+"/"+path);
		}
		};

	std::string search_path = app()->config->scripts.module_dir + "/.install";

	insert_dll_path(app()->config->scripts.module_dir);

	std::map<std::string, bool> dirs;
	ylib::file::list(search_path, dirs);
	for_iter(iter, dirs)
	{
		if (iter->second == false)
			continue;
		if (iter->first == "." || iter->first == "..")
			continue;

		insert_dll_path(search_path+"/"+iter->first);
	}

//	auto luas = ylib::file::traverse(app()->config->scripts.module_dir, "(.*\\.dll)");
	
	return results;
}