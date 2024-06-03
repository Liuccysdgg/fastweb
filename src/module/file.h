#pragma once
#include "sol/sol.hpp"
#include "imodule.h"

namespace module
{
	/// <summary>
	/// 文件
	/// </summary>
	class file:public module::imodule {
	public:
		static sol::table list(const std::string& dirpath,const std::string& regex,sol::this_state s);
		static void copy_dir(const std::string& src_dir,const std::string& dst_dir);
		static bool create_dir(const std::string& dirpath);
		static void regist(sol::state* lua);
	};

}

