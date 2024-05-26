#include "bytecodemanager.h"
#include "fastweb.h"
#include "utils/luautils.h"
#include "util/file.h"
#include "util/system.h"
#include "core/config.h"
bytecode_manager::bytecode_manager()
{
	::ithread::start();
}

bytecode_manager::~bytecode_manager()
{
	::ithread::stop();
	::ithread::wait();
}

bool bytecode_manager::create(const std::string& name, const std::string& filepath, bool auto_update)
{
	std::shared_ptr<bytecode> bc;
	if (m_bytescodes.get(name, bc))
	{
		m_lastErrorDesc = "The bytecode already exists, name: " + name;
		return false;
	}
	bc = std::make_shared<bytecode>();
	if (LuaUtils::make_bytecode(filepath, bc->value) == false)
	{
		m_lastErrorDesc = "make bytecode failed.";
		return false;
	}
	bc->filepath = filepath;
	bc->auto_update = true;
	bc->auto_update = auto_update;
	bc->pre_modify_msec = ylib::file::last_write_time(filepath);
	m_bytescodes.add(name, bc);
	return true;
}

const std::string& bytecode_manager::get(const std::string& name)
{
	static std::string return_empty;
	std::shared_ptr<bytecode> bc;
	if (m_bytescodes.get(name, bc) == false)
	{
		return return_empty;
	}
	return bc->value;
}

std::map<std::string, std::shared_ptr<bytecode>> bytecode_manager::map()
{
	return *m_bytescodes.parent();
}

bool bytecode_manager::run()
{
	m_bytescodes.lock();
	auto map = m_bytescodes.parent();
	for_iter(iter, (*map))
	{
		if (iter->second->auto_update == false)
			continue;

		auto last_write_time = ylib::file::last_write_time(iter->second->filepath);
		if (last_write_time != iter->second->pre_modify_msec)
		{
			LuaUtils::make_bytecode(iter->second->filepath, iter->second->value);
			iter->second->pre_modify_msec = last_write_time;
		}
	}
	m_bytescodes.unlock();

	system::sleep_msec(sConfig->scripts.auto_update_sec);

	return true;
}
