#pragma once
#include "sqlite3/sqlite3.h"
#include "base/define.h"
#include "base/error.h"
#include <vector>
#include <map>
#define SQLITE_RESULT std::vector<std::map<std::string, std::string>>
namespace ylib
{
	class sqlite3:public ylib::error_base
	{
	public:
		sqlite3();
		~sqlite3();
		bool open(const std::string& filepath,const std::string& username ="",const std::string& password = "");
		void close();
		bool exec(const std::string& sql);
		bool query(const std::string& sql, std::vector<std::map<std::string, std::string>>& data);
		int64 count(const std::string& sql);

		int64 last_insert_id();
	private:
		struct ::sqlite3* m_db;
	};
}