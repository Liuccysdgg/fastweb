#pragma once
#include <iostream>
#include "core/entry.h"
#include "util/system.h"
int main()
{
	std::string config_filepath = system::current_dir() + "/config.ini";
	if (fastweb(config_filepath.c_str()) != 0)
		return -1;

	while (true)
		std::cin.get();
	return 0;
}
