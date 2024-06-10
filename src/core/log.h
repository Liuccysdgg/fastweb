#pragma once
#include <string>
#include "util/file.h"
#include "core/define.h"
#include "util/thread.h"
#include "util/queue.hpp"
namespace fastweb
{
	class log :public Interface,private ylib::ithread {
	public:
		log(fastweb::app* ptr);
		~log();

		void success(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void info(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void error(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void warn(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void debug(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void lua(const std::string& msg);

		bool write();
	private:
		void print(const std::string& type, const std::string& msg, const std::string& filepath, const std::string& func, int line, int color, bool error);
		bool run() override;
	private:
		// 当前文件名
		std::string m_current_name;
		// 文件
		ylib::file_io m_file;
		// 日志队列
		ylib::queue<std::string> m_queue;
	};
}
