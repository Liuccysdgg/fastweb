#include "luautils.h"
#include "core/define.h"
#include "core/statemanager.h"
bool LuaUtils::make_bytecode(const std::string& filepath, std::string& bytecode)
{
	auto lua = sStateMgr->get_state();
	try
	{
		auto result = lua->load_file(filepath);
		if (!result.valid()) {
			sol::error err = result;
			LOG_ERROR(err.what() + std::string(", filepath: " + filepath));
			return false;
		}
		result();
		sol::function func = result;
		if (!func.valid()) {
			LOG_ERROR("Pre execution script failed before compilation. filepath: " + filepath);
			return false;
		}
		sol::function dump = (*lua)["string"]["dump"];
		sol::protected_function_result result2 = dump(func);
		if (!result2.valid()) {
			sol::error err = result2;
			LOG_ERROR("failed to dump. " + std::string(err.what()) + ", filepath: " + filepath);
			return false;
		}
		LOG_INFO("update lua script, filepath: " + filepath);
		bytecode = result2;
		return true;
	}
	catch (const std::exception& e)
	{
		LOG_INFO(std::string(e.what()) + ", filepath: " + filepath);
			}
	sStateMgr->push_state(lua);
	return false;
}
