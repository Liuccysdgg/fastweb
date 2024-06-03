#pragma once
#include "sol/sol.hpp"
#include "imodule.h"
#include "base/singleton.hpp"
#include "util/thread.h"
#include "util/queue.hpp"
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
	};
	/// <summary>
	/// 定时器
	/// </summary>
	class timer:public ylib::singleton<timer>,private ylib::ithread {
	public:
		timer();
		~timer();

		static std::string add(const std::string& name,const std::string& filepath,const std::string& funname,int msec,bool loop);
		static void remove(const std::string& name);


		static void regist(sol::state* lua);
	private:
	
		virtual bool run();
	public:
		std::map<std::string,timer_info> m_list;
		ylib::queue<std::string> m_removed;
		std::mutex m_mutex;
		bool m_insert = false;
	};

}

