#pragma once
#include <string>
namespace LogUtils {

	void success(const std::string& msg, const std::string& filepath, const std::string& func, int line);
	void info(const std::string& msg, const std::string& filepath, const std::string& func, int line);
	void error(const std::string& msg, const std::string& filepath, const std::string& func, int line);
	void warn(const std::string& msg, const std::string& filepath, const std::string& func, int line);
	void debug(const std::string& msg, const std::string& filepath, const std::string& func, int line);
}