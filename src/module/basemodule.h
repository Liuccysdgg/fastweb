#pragma once
#include "sol/sol.hpp"
#include "core/define.h"

namespace module
{
	/// <summary>
	/// 模块继承接口
	/// </summary>
	class base {
	public:
		virtual ~base() {};
		/// <summary>
		/// 注册全局变量(global类管理)
		/// </summary>
		/// <param name="name"></param>
		/// <param name="lua"></param>
		virtual void regist_global(const std::string& name,sol::state* lua) = 0;
		/// <summary>
		/// 释放全局变量(global类管理)
		/// </summary>
		virtual void delete_global() = 0;
		/// <summary>
		/// 全局变量阶段获取自身指针
		/// </summary>
		/// <returns></returns>
		virtual void* self() { return this; }
	};
}


