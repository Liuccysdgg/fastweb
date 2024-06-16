/*Software License

Copyright(C) 2024[liuyingjie]
License Terms
Usage Rights

Any individual or entity is free to use, copy, and distribute the binary form of this software without modification to the source code, without the need to disclose the source code.
If the source code is modified, the modifications must be open - sourced under the same license.This means that the modifications must be disclosed and accompanied by a copy of this license.
Future Versions Updates
From this version onwards, all future releases will be governed by the terms of the latest version of the license.This license will automatically be nullified and replaced by the new version.
Users must comply with the terms of the new license issued in future releases.
Liability and Disclaimer
This software is provided “as is”, without any express or implied warranties, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non - infringement.In no event shall the author or copyright holder be liable for any claims, damages, or other liabilities, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
Contact Information
If you have any questions, please contact us: 1585346868@qq.com Or visit our website fwlua.com.
*/
#pragma once
#include <iostream>
#include "core/entry.h"
#include <filesystem>
#include "util/file.h"
#include "util/strutils.h"
#include "util/system.h"
#include "slave.h"

#define FASTWEB_VERSION "V1.0.5"

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
	std::cout << "|------------------------[FASTWEB]----------------------------" << std::endl;
	std::cout << "| Version:\t"<< FASTWEB_VERSION << std::endl;
	std::cout << "| WorkingDirectory:\t"<< strutils::replace(system::current_dir(),'\\','/') << std::endl;
	std::cout << "| Args" << std::endl;
	for (int i = 0; i < argc; i++)
	{
		std::cout << "|\t" << argv[i] << std::endl;
	}
	std::cout << "|----------------------------------------------------------------" << std::endl;
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
	else if (type == "start2")
	{
		if (ylib::file::exist(std::filesystem::absolute(value).string()))
		{
			app = start(std::filesystem::absolute(value).string().c_str());
			if (app == nullptr)
				return -1;
			while (true)
				system::sleep_msec(1000 * 60);
		}
		else
			return -1;
	}
	else if (type == "fastwebmanager")
	{
		int flag = ylib::stoi(argv[2]);
		slave s("fastweb_shardmemory", flag);

		app = start(argv[3]);
		if (app == nullptr)
		{
			s.write(1);
			return -1;
		}
		s.write(2);
		// 等待关闭信号
		s.wait();

		return 0;
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
	return -1;
}
