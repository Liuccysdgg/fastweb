#pragma once


#include "sol/sol.hpp"

#include "base/error.h"
#include "util/thread.h"
#include "util/queue.hpp"
#include "util/map.hpp"
#include "core/structs.h"
#include "core/define.h"

namespace fastweb
{
	/// <summary>
	/// LUALIB库变动检测
	/// </summary>
	class lualib_detecter:public Interface {
	public:
		lualib_detecter(fastweb::app* app);
		~lualib_detecter();
		/// <summary>
		/// 是否变化
		/// </summary>
		/// <returns></returns>
		bool changed();
	private:
		std::map<std::string, timestamp> m_files;
	};
}
