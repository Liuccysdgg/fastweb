#pragma once
#include <map>
#include "sol/sol.hpp"
#include "core/structs.h"
#include "core/define.h"
typedef int (*fastweb_module_regist)(void*, void*);
typedef int (*lua_interface)(void*);
struct module_info {
	void* dll = nullptr;
	fastweb_module_regist func = nullptr;
};
namespace fastweb
{
	/// <summary>
	/// 模块管理器
	/// </summary>
	class module_manager :public Interface {
	public:
		module_manager(fastweb::app* app);
		~module_manager();

		void start();
		void close();
		/// <summary>
		/// 加载虚拟机库
		/// </summary>
		/// <returns></returns>
		void load(sol::state* lua);
	private:
		/// <summary>
		/// 加载核心库
		/// </summary>
		/// <param name="lua"></param>
		void load_core(sol::state* lua);
		/// <summary>
		/// 加载三方库
		/// </summary>
		void load_3rdparty(sol::state* lua);
		/// <summary>
		/// 加载LUA库
		/// </summary>
		/// <param name="lua"></param>
		void load_lualib(sol::state* lua);
		/// <summary>
		/// 取模块文件列表
		/// </summary>
		/// <returns></returns>
		std::vector<std::string> modules();
	private:
		// DLL模块
		std::map<std::string, module_info> m_modules;
		// LUA虚拟机引用
		std::string m_lua_include_path;
		std::string m_lua_include_cpath;
	};
}
