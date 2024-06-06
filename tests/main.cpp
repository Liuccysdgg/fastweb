#pragma once
#include <iostream>
#include "core/entry.h"
#include <filesystem>
std::string config_filepath;
void* fastweb_app_ptr = nullptr;
void start()
{
	fastweb_app_ptr = fastweb_start(config_filepath.c_str());
}
void close()
{
	if(fastweb_app_ptr == nullptr)
		fastweb_close(fastweb_app_ptr);
	fastweb_app_ptr = nullptr;
}
int main()
{
	config_filepath = std::filesystem::current_path().string() + "/config.ini";

	std::string input = "restart";
	while (true)
	{
		if (input == "restart")
		{
			std::cout << "closing..." << std::endl;
			close();
			std::cout << "starting..." << std::endl;
			start();
			if (fastweb_app_ptr == nullptr)
			{
				std::cin.get();
				return -1;
			}
			std::cout << "started" << std::endl;
		}
		else if (input == "quit" || input == "exit")
		{
			close();
			std::cout << "closed";
			break;
		}
		else
		{
			std::cout << "Enter \"quit\" or \"exit\" to exit the application" << std::endl;
			std::cout << "Enter \"restart\"  to restart the application" << std::endl;
		}


		std::cin >> input;
	}
	return 0;
}
