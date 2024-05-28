#include <iostream>
#include "core/fastweb.h"
#include "util/system.h"
#include "core/define.h"
#include "core/config.h"
extern "C" {
	int fastweb(const char* config_filepath)
	{
		std::cout << "=========== [fastweb engine] ============" << std::endl;
		if (sConfig->open(config_filepath) == false)
		{
			LOG_ERROR("open config failed," + sConfig->last_error());
			return -1;
		}

		if (fastweb::getInstance()->start() == false)
		{
			LOG_ERROR("fastweb start failed," + fastweb::getInstance()->last_error());
			return -1;
		}
		return 0;
	}
}