#include "modulemanager.h"

#include "util/file.h"

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
#include "module/mysql.h"
#ifdef _WIN32
#include "module/mssql.h"
#endif
#include "module/localstorage.h"
#include "module/globalfuns.h"
#include "module/mutex.h"
#include "module/codec.h"
#include "module/time.h"
#include "module/file.h"
#include "module/sys.h"
#include "module/timer.h"
module_manager::module_manager()
{
}
void module_manager::start()
{
	close();
	auto ms = modules();
	for (size_t i = 0; i < ms.size(); i++)
	{
		module_info mi;
		std::string mod_filepath = sConfig->scripts.module_dir + "/" + ms[i];
#ifdef _WIN32
		mi.dll = LoadLibrary(mod_filepath.c_str());
		if (mi.dll == nullptr)
		{
			LOG_ERROR("module loading failed, filename: " + mod_filepath);
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

void module_manager::close()
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

void module_manager::load(sol::state* lua)
{
	load_core(lua);
	load_lualib(lua);
	load_3rdparty(lua);
}

std::string module_manager::search(const std::string& filepath)
{
	for (size_t i = 0; i < sConfig->scripts.lib_dir.size(); i++)
	{
		std::string path = sConfig->scripts.lib_dir[i] + "/" + filepath;
		if (ylib::file::exist(path))
			return path;
	}
	return "";
}

void module_manager::load_core(sol::state* lua)
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
	module::session::regist(lua);
	module::httpclient::regist(lua);
	module::mysql_regist(lua);
#ifdef _WIN32
	module::mssql::regist(lua);
#endif
	module::regist_globalfuns(lua);
	module::local_storage::regist(lua);
	module::mutex::regist(lua);
	module::auto_lock::regist(lua);
	module::codec::regist(lua);
	module::time::regist(lua);
	module::file::regist(lua);
	module::sys::regist(lua);
	module::timer::regist(lua);

	global::getInstance()->regist_lua(lua);

}

void module_manager::load_3rdparty(sol::state* lua)
{
	for_iter(iter, m_modules)
	{ 
		if (iter->second.func(lua, lua->lua_state()) != 0)
		{
			LOG_ERROR("egist module failed, filename: "+iter->first);
		}
	}
}

void module_manager::load_lualib(sol::state* lua)
{
	// 获取当前的package.path，添加新的搜索路径
	std::string current_path = (*lua)["package"]["path"];  // 获取当前的路径
	for (size_t i = 0; i < sConfig->scripts.lib_dir.size(); i++)
		current_path += ";" + sConfig->scripts.lib_dir[i] + "/?.lua";  // 添加新的路径
	(*lua)["package"]["path"] = current_path;  // 设置修改后的路径
}

std::vector<std::string> module_manager::modules()
{
	std::vector<std::string> results;
	auto luas = ylib::file::traverse(sConfig->scripts.module_dir, "(.*\\.dll)");
	for_iter(iter, luas)
	{
		if (iter->second == IS_DIRECTORY)
			continue;
		std::string path = strutils::replace(iter->first, '\\', '/');
		results.push_back(path);
	}
	return results;
}