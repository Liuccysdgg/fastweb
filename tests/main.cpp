#pragma once
#include <iostream>
#include "core/entry.h"
#include <filesystem>
int main()
{
	std::string config_filepath = std::filesystem::current_path().string()+"/config.ini";
	if (fastweb(config_filepath.c_str()) != 0)
		return -1;

	while (true)
		std::cin.get();
	return 0;
}
