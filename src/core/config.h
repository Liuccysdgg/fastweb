#pragma once
#include "define.h"
#include "base/error.h"
#include "base/singleton.hpp"
#include "util/ini.h"
#include "net/http_define.h"
class config:public ylib::error_base,public ylib::singleton<config>{
public:
	struct domain {
		bool https = false;
		ushort port = 0;
		network::http::ssl_config ssl;
	};
	struct scripts {
		std::string app_dir;
		std::string lib_dir;
		std::string module_dir;
		uint32 lua_cache_size = 0;
		std::string app_mapping_dir;
		uint32 auto_update_sec = 0;
	};
	struct website {
		struct __interceptor {
			std::string filepath;
			std::string regex_express;
		};
		std::string static_dir;
		std::string default_404;
		std::vector<std::string> default_index;
		std::string session_dir;
		uint32 session_timeout_sec;
		std::string Initialization_script;
		std::vector<__interceptor> interceptor_scripts;
		bool debug = false;
		std::vector<std::string> domain;
	};
public:
	config() = default;
	bool open(const std::string& ini_filepath);

	std::vector<std::string> lua_app_files();
	std::vector<std::string> lua_lib_files();
private:
	// INI配置文件
	ylib::ini m_ini;
private:
	/// <summary>
	/// 查询所有变量字符串
	/// </summary>
	/// <param name="text"></param>
	/// <returns></returns>
	std::vector<std::string> extractVariableNames(const std::string& text);
	/// <summary>
	/// 缓存配置
	/// </summary>
	void cache();
public:
	scripts scripts;
	website website;
	std::map<std::string, domain> domain;
};