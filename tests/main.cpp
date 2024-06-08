#pragma once
#include <iostream>
#include "core/entry.h"
#include <filesystem>
#include "util/file.h"
#include "util/strutils.h"
#define QUIT_WAIT 				std::cout << "Please exit after entering any character...";std::cin.get();return -1

void* start(const char* config_filepath)
{
	return fastweb_start(config_filepath);
}
void close(void* app)
{
	if(app == nullptr)
		fastweb_close(app);
}
void ouput_help()
{
	std::cerr << "No configuration file path provided. Please provide the path to a config.ini file as an argument." << std::endl;

	std::cout << "------------------------------------------------------------------------------" << std::endl;
	std::cout << "| The instructions are as follows:" << std::endl;
	std::cout << "| 1、fastweb start config.ini\t\t [load the specified configuration and start Fastweb]" << std::endl;
	std::cout << "| 2、fastweb create config .\t\t [create a default configuration file]" << std::endl;
	std::cout << "| 3、fastweb create website .\t\t [create a default website directory]" << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
}
int main(int argc, char* argv[]) 
{
	if (argc < 2) { ouput_help(); QUIT_WAIT; }
	std::string type = argv[1];
	if (type == "help")
	{
		ouput_help();
		QUIT_WAIT;
	}

	if (argc < 3){ouput_help();QUIT_WAIT;}

	void* app = nullptr;
	std::string value = argv[2];
	if (type == "start")
	{
		if (ylib::file::exist(std::filesystem::absolute(value).string()))
		{
			app = start(std::filesystem::absolute(value).string().c_str());
			if (app == nullptr)
			{
				QUIT_WAIT;
			}
			while (true) {
				std::string input;
				std::cin >> input;
				if (input == "quit" || input == "exit")
				{
					close(app);
					std::cout << "closed";
					return 0;
				}
				else
				{
					std::cout << "Enter \"quit\" or \"exit\" to exit the application" << std::endl;
					std::cout << "Enter \"restart\"  to restart the application" << std::endl;
				}
			}
		}
		else
		{
			std::cerr << "file not found: " << std::filesystem::absolute(value).string() << std::endl;
			QUIT_WAIT;
		}
	}
	if (argc < 4) { ouput_help(); QUIT_WAIT; }

	if (type == "create")
	{
		std::string path = std::filesystem::absolute(argv[3]).string();
		if (value == "config")
		{
			ylib::file::copy("/usr/local/share/fastweb/config.ini",path);
		}
		else if (value == "website")
		{
			ylib::file::copy_dir("/usr/local/share/fastweb", path);
		}
	}
	return 0;
}
