#pragma once
#include "sol/sol.hpp"
#include "imodule.h"

namespace module
{
	/// <summary>
	/// 系统
	/// </summary>
	class sys:public module::imodule {
	public:
		static std::string current_dir();
		static void sleep_msec(uint32 msec);
		static int64 random(int64 min, int64 max);
		static std::string current_filepath();
		static std::string temp_path();
		static std::string desktop_path();
		static std::string currentuser_path();
		static sol::table mac(sol::this_state s);

		static void regist(sol::state* lua);
	};

}

