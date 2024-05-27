#pragma once
#include "sol/sol.hpp"
#include "imodule.h"

namespace module
{
	/// <summary>
	/// 编解码
	/// </summary>
	class codec:public module::imodule {
	public:
		/// <summary>
		/// URL解码
		/// </summary>
		static std::string url_de(const std::string& value);
		/// <summary>
		/// URL编码
		/// </summary>
		static std::string url_en(const std::string& value);
		/// <summary>
		/// GBK转UTF8
		/// </summary>
		/// <returns></returns>
		static std::string gbk_to_utf8(const std::string& value);
		static std::string utf8_to_gbk(const std::string& value);
		/// <summary>
		/// MD5校验
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		static std::string md5(const std::string& value);

		static void regist(sol::state* lua);
	};

}

