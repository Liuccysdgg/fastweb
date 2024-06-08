#include "subscribe.h"
#include "core/app.h"

module::subscribe::subscribe()
{
}

module::subscribe::~subscribe()
{
}

bool module::subscribe::add(const std::string& express_string, const std::string& filepath, sol::this_state ts)
{
	GET_APP;
	return app->subscribe->add(express_string,filepath);
}

bool module::subscribe::remove(const std::string& express_string, sol::this_state ts)
{
	GET_APP;
	return app->subscribe->remove(express_string);
}

bool module::subscribe::exist(const std::string& express_string, sol::this_state ts)
{
	GET_APP;
	return app->subscribe->exist(express_string);
}

void module::subscribe::clear(sol::this_state ts)
{
	GET_APP;
	return app->subscribe->clear();
}

void module::subscribe::regist(sol::state* lua)
{
	lua->new_usertype<module::subscribe>("subscribe",
		"add", &module::subscribe::add,
		"remove", &module::subscribe::remove,
		"exist", &module::subscribe::exist,
		"clear", &module::subscribe::clear
	);
}
