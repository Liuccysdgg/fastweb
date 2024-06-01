#pragma once
#ifdef _WIN32
#include "sol/sol.hpp"
#include "imodule.h"
#include "soci/soci.h"
namespace module
{
	/// <summary>
	/// 结果集
	/// </summary>
	class mssql
	{
	public:
		mssql(const std::string& connstring);
		~mssql();

		/// <summary>
		/// 取数据
		/// </summary>
		std::string get_str(const std::string& name);
		std::string get_datetime(const std::string& name);
		int32 get_i32(const std::string& name);
		int64 get_i64(const std::string& name);
		double get_dob(const std::string& name);
		void query(const std::string& sql);
		/// <summary>
		/// 下一行
		/// </summary>
		bool next();


		int64 update(const std::string& sql);
		/// <summary>
		/// 注册
		/// </summary>
		/// <param name="lua"></param>
		static void regist(sol::state* lua);
	private:
		std::shared_ptr<soci::session> m_session;
		//std::shared_ptr<soci::statement> m_st;
		soci::rowset_iterator<soci::row> m_iter;
		soci::rowset<soci::row> m_rows;
		bool m_read = false;
	};


}

#endif