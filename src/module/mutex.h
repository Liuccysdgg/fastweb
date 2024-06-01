#pragma once
#include "sol/sol.hpp"
#include "imodule.h"

namespace module
{
	/// <summary>
	/// 互斥锁
	/// </summary>
	class mutex:public module::imodule {
	public:
		mutex();
		~mutex() override;
		/// <summary>
		/// 加锁
		/// </summary>
		void lock();
		/// <summary>
		/// 释放锁
		/// </summary>
		void unlock();
		/// <summary>
		/// 尝试加锁
		/// </summary>
		/// <returns></returns>
		bool try_lock();
		static void regist(sol::state* lua);
	private:
		// 通过 imodule 继承
		virtual void regist_global(const std::string& name, sol::state* lua);
		virtual void delete_global() { delete this; }
		std::mutex m_mutex;
	};

	/// <summary>
	/// 自动锁
	/// </summary>
	class auto_lock 
	{
	public:
		auto_lock(module::mutex& mutex);
		~auto_lock();
		static void regist(sol::state* lua);
	private:
		module::mutex *m_mutex = nullptr;
	};
}

