#pragma once
#include "sol/sol.hpp"
#include "imodule.h"

namespace module
{
	/// <summary>
	/// 时间
	/// </summary>
	class time:public module::imodule {
	public:
		/// <summary>
		/// URL解码
		/// </summary>
		static uint64 now_msec();
		static uint64 now_sec();

		static void regist(sol::state* lua);
	};

}

