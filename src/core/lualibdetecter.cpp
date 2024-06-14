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

#include "lualibdetecter.h"
#include "core/config.h"
#include "core/app.h"
#include "util/file.h"
fastweb::lualib_detecter::lualib_detecter(fastweb::app* app):Interface(app)
{
}

fastweb::lualib_detecter::~lualib_detecter()
{
}

bool fastweb::lualib_detecter::changed()
{
	auto lib_files = app()->config->lua_lib_files();
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
