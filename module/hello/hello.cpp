#include "hello.h"
#include "module.h"
#include "sol/sol.hpp"
hello::hello()
{
}

hello::~hello()
{
}

std::string hello::name()
{
	return "My name is `Fast Web`";
}


extern "C" {
#ifdef _WIN32
	DLL_EXPORT
#endif
	int fastweb_module_regist(void* sol2, void* lua)
	{
		sol::state* state = static_cast<sol::state*>(sol2);
		state->new_usertype<hello>("hello",
			"name", &hello::name
		);
		// 返回成功
		return 0;
	}
}


