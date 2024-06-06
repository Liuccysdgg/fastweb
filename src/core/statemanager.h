#pragma once


#include "sol/sol.hpp"

#include "base/error.h"
#include "util/thread.h"
#include "util/queue.hpp"
#include "util/map.hpp"

#include "core/structs.h"
#include "core/lualibdetecter.h"
#include "core/modulemanager.h"
namespace fastweb
{
	/// <summary>
	/// LUA状态管理器
	/// </summary>
	class state_manager :private ylib::ithread,public Interface {
	public:
		state_manager(fastweb::app* app);
		~state_manager();
		/// <summary>
		/// 启动
		/// </summary>
		bool start();
		void close();
		/// <summary>
		/// 取虚拟机
		/// </summary>
		/// <returns></returns>
		luastate* get();
		/// <summary>
		/// 归还虚拟机
		/// </summary>
		/// <param name="state"></param>
		void push(luastate* state);
	public:
		// 模块管理器
		std::shared_ptr<fastweb::module_manager> module_manager;
	private:
		// 虚拟机
		ylib::queue<luastate*> m_states;
		// 版本FLAT
		size_t m_flag = 0;
		// LIB变化检测
		std::shared_ptr<fastweb::lualib_detecter> lib_detecter;
		
	private:
		// 通过 ithread 继承
		bool run() override;
		/// <summary>
		/// 创建虚拟机
		/// </summary>
		/// <returns></returns>
		luastate* create();




	};
}
