#pragma once
#include <iostream>
#include "core/entry.h"
#include <filesystem>
int main()
{
	std::string config_filepath = std::filesystem::current_path().string()+"/config.ini";
	if (fastweb_start(config_filepath.c_str()) != 0)
		return -1;

	while (true)
	{
		std::string input;
		std::cin >> input;

		if (input == "quit" || input == "exit")
		{
			fastweb_close();
			std::cout << "closed";
			break;
		}
		else
			std::cout << "Enter \"quit\" or \"exit\" to exit the application" << std::endl;
	}
	return 0;
}
