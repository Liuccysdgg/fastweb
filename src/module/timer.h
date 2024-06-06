#pragma once
#include "sol/sol.hpp"
#include "core/structs.h"
#include "util/thread.h"
#include "util/queue.hpp"
#include "module/basemodule.h"
#include <map>
#include <mutex>
namespace module
{
	struct timer_info {
		std::string filepath;
		std::string name;
		std::string funname;
		bool loop = false;
		timestamp exec_msec = 0;
		timestamp msec = 0;
		luastate* lua = nullptr;
		sol::function function;
	};
	/// <summary>
	/// 定时器
	/// </summary>
	class timer:private ylib::ithread,public module::base {
	public:
		timer();
		~timer();
		/// <summary>
		/// 增加
		/// </summary>
		/// <param name="name"></param>
		/// <param name="filepath"></param>
		/// <param name="funname"></param>
		/// <param name="msec"></param>
		/// <param name="loop"></param>
		/// <returns></returns>
		std::string add(const std::string& name,const std::string& filepath,const std::string& funname,int msec,bool loop, sol::this_state ts);
		/// <summary>
		/// 移除
		/// </summary>
		/// <param name="name"></param>
		void remove(const std::string& name, sol::this_state ts);
		static void regist(sol::state* lua);
		virtual void regist_global(const std::string& name, sol::state* lua) override;
		virtual void delete_global() { delete this; }
	private:
		
		virtual bool run();
	public:
		fastweb::app* m_app = nullptr;
		std::map<std::string,timer_info> m_list;
		ylib::queue<std::string> m_removed;
		std::mutex m_mutex;
		bool m_insert = false;
	};

}

