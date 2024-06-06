#pragma once
#include "define.h"
#include "base/error.h"
#include "util/map.hpp"
namespace module
{
	class base;
}
namespace fastweb
{
	/// <summary>
	/// 全局托管模块(程序启动创建、程序销毁释放)
	/// </summary>
	class global_module : public Interface{
	public:
		global_module(fastweb::app* app);
		~global_module();
		/// <summary>
		/// 创建
		/// </summary>
		/// <param name="module_ptr"></param>
		bool regist(const std::string& name,module::base* module_ptr);
		/// <summary>
		/// 清理
		/// </summary>
		void clear();
	private:
		std::map<std::string, module::base*> m_ptrs;
	};
}


