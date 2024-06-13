#pragma once
#include <string>
#include "util/file.h"
#include "core/define.h"
#include "util/thread.h"
#include "util/queue.hpp"
#include "util/sqlite3.h"
namespace fastweb
{
	class log :public Interface,private ylib::ithread {
		
	public:
		enum log_type {
			LT_SUCCESS = 1,
			LT_INFO =  2,
			LT_ERROR =  3,
			LT_WARN = 4,
			LT_DEBUG =  5,
			LT_LUA = 6
		};
	private:
		struct log_info {
			log_type type = LT_SUCCESS;
			std::string msg;
			std::string filepath;
			std::string function;
			std::string create_at;
			int line;
		};
	public:
		log(fastweb::app* ptr);
		~log();

		void start();

		void success(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void info(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void error(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void warn(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void debug(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void lua(const std::string& msg);

		bool write();
	private:
		void print(log_type type, const std::string& msg, const std::string& filepath, const std::string& func, int line, int color, bool error);
		bool run() override;


		
	private:
		// 当前文件名
		std::string m_current_name;
		// 文件
		ylib::file_io m_file;
		// SQLITE
		ylib::sqlite3 m_sqlite;
		// 日志队列
		ylib::queue<log_info> m_queue;
	};
}
