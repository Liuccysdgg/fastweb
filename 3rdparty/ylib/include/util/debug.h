#pragma once
#include <string>
namespace ylib
{
	namespace debug
	{
		// 检测异常产生DUMP文件
		void detect_exception();
#ifdef _WIN32
		// 打印VS控制台
		void vs_console_println(const std::string& value);
#endif
	}
}
