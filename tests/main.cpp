
// MIT License﻿
// Copyright(c) 2024 FastWeb - fwlua.com - nianhua﻿
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :﻿
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.﻿
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.﻿
// ## Additional Terms for Commercial Use﻿
// This software is licensed for personal, educational, and non - commercial use.
// For commercial use or use within a company, organization, or institution, a
// separate commercial license is required.To obtain a commercial license,
// please contact
// EMail：1585346868@qq.com
// Mobile：17367918735
// QQ：1585346868


#pragma once
#include <iostream>
#include "core/entry.h"
#include <filesystem>
#include "util/file.h"
#include "util/strutils.h"
#include "util/system.h"
#include "slave.h"
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
