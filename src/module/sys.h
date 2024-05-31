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
		static void regist(sol::state* lua);
	};

}

