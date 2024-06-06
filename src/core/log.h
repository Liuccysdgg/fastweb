#pragma once
#include <string>
#include "util/file.h"
#include "core/define.h"
namespace fastweb
{
	class log :public Interface {
	public:
		log(fastweb::app* ptr);
		~log();

		void success(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void info(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void error(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void warn(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void debug(const std::string& msg, const std::string& filepath, const std::string& func, int line);
		void lua(const std::string& msg);
	private:
		void print(const std::string& type, const std::string& msg, const std::string& filepath, const std::string& func, int line, int color, bool error);
	private:
		// 当前文件名
		std::string m_current_name;
		// 文件
		ylib::file_io m_file;
	};
}
