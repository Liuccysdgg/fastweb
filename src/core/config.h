﻿#pragma once
#include "define.h"
#include "base/error.h"
#include "util/ini.h"
#include "net/http_define.h"
namespace fastweb
{
	class config :public Interface {
	public:
		struct domain {
			bool https = false;
			ushort port = 0;
			network::http::ssl_config ssl;
		};
		struct scripts {
			//std::vector<std::string> lib_dir;
			std::string module_dir;
			uint32 lua_cache_size = 0;
			uint32 auto_update_sec = 0;
		};
		struct website {
			struct __interceptor {
				std::string filepath;
				std::string regex_express;
			};
			std::string dir;
			std::string default_404;
			std::vector<std::string> default_index;
			std::string session_dir;
			uint32 session_timeout_sec;
			std::string Initialization_script;
			bool debug = false;
			std::vector<std::string> domain;
			bool direct_url_mapping = false;
			uint64 max_upload_size = 0;
		};

		struct log {
			bool enable = false;
			std::string dir;
			std::string name;
			bool sqlite = false;
			/*bool succ = false;
			bool info = false;
			bool warn = false;
			bool error = false;*/
		};
	public:
		config(fastweb::app* ptr);
		bool open(const std::string& ini_filepath);
		std::vector<std::string> lua_lib_files();


		void write_runtime(const ylib::json& data);
		ylib::json read_runtime();

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
		std::string m_ini_filepath;
		scripts scripts;
		website website;
		log log;
		std::map<std::string, domain> domain;


	};
}
