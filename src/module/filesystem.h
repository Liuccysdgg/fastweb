#pragma once
#include "sol/sol.hpp"
#include "core/define.h"
namespace module
{
	/// <summary>
	/// 文件
	/// </summary>
	class filesystem {
	public:
		static sol::table list(const std::string& dirpath,const std::string& regex,sol::this_state s);
		static void copy_dir(const std::string& src_dir,const std::string& dst_dir);
		static bool create_dir(const std::string& dirpath);
		static sol::object read(const std::string& filepath,sol::this_state s);
		static bool write(const std::string& filepath,const std::string& data);
		static bool remove(const std::string& filepath);
		static bool remove_dir(const std::string& dirpath, bool recycle);
		static bool exist(const std::string& filepath);
		static bool exist_dir(const std::string& dirpath);
		static int64 size(const std::string& filepath);
		static bool copy(const std::string& src, const std::string& dst);
		static timestamp last_write_time(const std::string& filepath);

		static void regist(sol::state* lua);
	};

}

