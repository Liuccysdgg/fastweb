#include "lualibdetecter.h"
#include "core/config.h"
#include "util/file.h"
lualib_detecter::lualib_detecter()
{
}

lualib_detecter::~lualib_detecter()
{
}

bool lualib_detecter::changed()
{
	auto lib_files = sConfig->lua_lib_files();
	bool changed = false;
	if (lib_files.size() == m_files.size())
	{
		for (size_t i = 0; i < lib_files.size(); i++)
		{
			auto iter = m_files.find(lib_files[i]);
			if (iter == m_files.end())
			{
				changed = true;
				break;
			}
			if (ylib::file::last_write_time(lib_files[i]) != iter->second)
			{
				changed = true;
				break;
			}
		}
	}
	else
		changed = true;


	if (changed == false)
		return false;

	m_files.clear();
	for (size_t i = 0; i < lib_files.size(); i++)
		m_files.emplace(lib_files[i], ylib::file::last_write_time(lib_files[i]));

	return true;
}
