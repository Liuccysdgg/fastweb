#pragma once
#include "sol/sol.hpp"
#include "util/ini.h"
namespace module
{
	/// <summary>
	/// INI配置
	/// </summary>
	class ini {
	public:
		ini();
		~ini();
		bool open(const std::string& filepath);
		void close();
		std::string read(const std::string& node, const std::string& key, const std::string& default_value) const;
		bool write(const std::string& node, const std::string& key, const std::string& value);
		bool del(const std::string& node, const std::string& key);
		sol::table nodes(sol::this_state s);
		sol::table keys(const std::string& node, sol::this_state s);
		bool exist_key(const std::string& node, const std::string& key);
		bool exist_node(const std::string& node);
		sol::table table(sol::this_state s);

		static void regist(sol::state* lua);
	private:
		ylib::ini m_ini;
	};

}

