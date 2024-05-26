#pragma once
#include "base/buffer.h"
#include "util/map.hpp"
namespace ylib
{
	class package
	{
	public:
		struct packstruct
		{
			std::string name;
			ylib::buffer data;
		};
	public:
		package();
		~package();
		bool parse(const ylib::buffer& data);
		bool add(const std::string& name, const ylib::buffer& data);
		void to(const std::string& password, ylib::buffer& data);
		bool get(const std::string& name, ylib::buffer& data);
		void clear();

		void ___test__c_to_utf8();
	private:
		void free();
	private:
		ylib::map<std::string, packstruct*> m_list;
		ylib::map<std::string, ylib::buffer*> m_parse_list;
	};
}

