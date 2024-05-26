#pragma once
#include <string>
#include "base/define.h"
namespace ylib
{
	void println(const std::string& text,ylib::ConsoleTextColor color = ylib::ConsoleTextColor::DEFAULT);
	void print(const std::string& text, ylib::ConsoleTextColor color = ylib::ConsoleTextColor::DEFAULT);
}


