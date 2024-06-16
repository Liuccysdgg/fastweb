#pragma once
#include "sol/sol.hpp"
#include "core/define.h"
namespace module
{
	/// <summary>
	/// 系统
	/// </summary>
	class sys {
	public:
		static void sleep_msec(uint32 msec);

		static void regist(sol::state* lua);
	};

}

