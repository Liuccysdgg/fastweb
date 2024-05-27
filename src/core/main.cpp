#pragma once
#include <iostream>
#include "core/fastweb.h"
#include "util/system.h"
#include "core/define.h"
#include "core/config.h"
int main()
{
	std::cout << "=========== [fastweb engine] ============" << std::endl;


	if(sConfig->open(system::current_dir() + "\\config.ini") == false)
	{
		LOG_ERROR("open config failed,"+sConfig->last_error());
		return -1;
	}

	if (fastweb::getInstance()->start() == false)
	{
		LOG_ERROR("fastweb start failed,"+fastweb::getInstance()->last_error());
		return -1;
	}

	LOG_SUCC("success");
	while (true)
		std::cin.get();
	return 0;
}