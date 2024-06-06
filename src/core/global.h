#pragma once
#include "define.h"
#include "base/error.h"
#include "util/map.hpp"
namespace fastweb
{
	/// <summary>
	/// 应用全局变量
	/// </summary>
	class global : public Interface{
	public:
		global(fastweb::app* app);
		~global();
		/// <summary>
		/// 注册变量
		/// </summary>
		/// <param name="lua"></param>
		void regist(sol::state* lua);
		/// <summary>
		/// 取指针
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		void* get_ptr(const std::string& name);
		/// <summary>
		/// 置指针
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <param name="ts"></param>
		/// <returns></returns>
		bool set_ptr(const std::string& name, void* value, sol::this_state ts);

		/// <summary>
		/// 取对象
		/// </summary>
		/// <param name="name"></param>
		/// <param name="s"></param>
		/// <returns></returns>
		sol::object get_obj(const std::string& name, sol::this_state s);
		/// <summary>
		/// 置对象
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void set_obj(const std::string& name, sol::object value);
		/// <summary>
		/// 清理
		/// </summary>
		void clear();
	private:
		ylib::map<std::string, void*> m_ptrs;
		ylib::map<std::string, sol::object> m_values;
	};
}


