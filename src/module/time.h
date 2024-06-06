#pragma once
#include "sol/sol.hpp"
#include "core/define.h"
namespace module
{
	/// <summary>
	/// 时间
	/// </summary>
	class time {
	public:
		static uint64 now_msec();
		static uint64 now_sec();
		static std::string now_time(const std::string& format);
		static void regist(sol::state* lua);
	};

}

