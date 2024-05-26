#pragma once
#include <string>
#include <vector>
#include "util/json.h"
namespace ylib
{
	class ini
	{
	public: 
		ini();
        ~ini(); 
        bool open(const std::string& filepath);
		void close();
        std::string read(const std::string& name,const std::string& key,const std::string& default_value = "") const;
        bool write(const std::string& name,const std::string& key,const std::string& value);
#ifndef _WIN32
		bool del(const std::string& name, const std::string& key);

		// 一级NAME
		std::vector<std::string> names();
		// 二级KEY
		std::vector<std::string> keys(const std::string& name);

		bool exist_key(const std::string& name,const std::string& key);
		bool exist_name(const std::string& name);
		ylib::json to_json();
#endif
	private:
        std::string m_filepath;
		void* m_point = nullptr;
	};
}


