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
			auto a = static_cast<fastweb::app*>(app);
			a->stop();
			delete a;
		}
}

