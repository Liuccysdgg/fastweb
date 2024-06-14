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

#include <iostream>
#include "core/app.h"
#include "util/system.h"
#include "core/define.h"
#include "core/config.h"
#include "entry.h"
extern "C" { 
#ifdef _WIN32
	DLL_EXPORT
#endif
	void* fastweb_start(const char* config_filepath)
	{
		fastweb::app* app = new fastweb::app();
		if (app->start(config_filepath) == false)
		{

			app->log->error("fastweb start failed," + app->last_error(), __FILE__, __func__, __LINE__);
			app->log->write();
			delete app;
			return nullptr;
		}
		app->log->success("success", __FILE__, __func__, __LINE__);
		return app;
	}
#ifdef _WIN32
	DLL_EXPORT
#endif
	void fastweb_close(void* app)
	{
		if (app == nullptr)
			return;
		auto a = static_cast<fastweb::app*>(app);
		a->stop();
		delete a;
	}
}

