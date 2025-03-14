#pragma once
#include "sol/sol.hpp"
#include "basemodule.h"

namespace module
{
	/// <summary>
	/// 编解码
	/// </summary>
	class codec{
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
		static std::string to_utf8(const std::string& value);
		static std::string to_gbk(const std::string& value);
		/// <summary>
		/// MD5校验
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		static std::string md5(const std::string& value);

		static std::string sha1(const std::string_view& value);
		static std::string sha256(const std::string_view& value);
		static std::string hmac_sha256(const std::string_view& key,const std::string_view& value);

		static std::string aes_en(const std::string_view& value, const std::string_view& key, const std::string& variant,const std::string& mode);
		static std::string aes_de(const std::string_view& value,const std::string_view& key, const std::string& variant, const std::string& mode);


		static void regist(sol::state* lua);
	};

}

