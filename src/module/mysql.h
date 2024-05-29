#pragma once
#include "db/mysql.h"
#include "db/sqler.h"
#include "sol/sol.hpp"
#include "imodule.h"
namespace module
{
	/// <summary>
	/// 注册
	/// </summary>
	/// <param name="lua"></param>
	void mysql_regist(sol::state& lua);
	/// <summary>
	/// 结果集
	/// </summary>
	class mysql_result
	{
	public:
		mysql_result(ylib::mysql::result* result);
		~mysql_result();
		/// <summary>
		/// 字段数
		/// </summary>
		/// <returns></returns>
		uint32 field_count();
		/// <summary>
		/// 字段类型
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		std::string field_type(sol::object obj);
		/// <summary>
		/// 字段名称
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		std::string field_name(uint32 index);
		/// <summary>
		/// 结果行
		/// </summary>
		/// <returns></returns>
		size_t row_count();
		/// <summary>
		/// 下一行
		/// </summary>
		/// <returns></returns>
		bool next();
		/// <summary>
		/// 取数据
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		VarType get(sol::object obj, sol::this_state s);
		/// <summary>
		/// 取结果集到table
		/// </summary>
		/// <param name="s"></param>
		/// <returns></returns>
		sol::table table(sol::this_state s);
		/// <summary>
		/// 注册
		/// </summary>
		/// <param name="lua"></param>
		static void regist(sol::state& lua);
	private:
		ylib::mysql::result* m_result = nullptr;
	};


	class select {
	public:
		select(ylib::mysql::conn* conn);
		~select();
		module::select& where_i32(const std::string& name, const std::string& expression, int32 value);
		module::select& where_i64(const std::string& name, const std::string& expression, int64 value);
		module::select& where_dob(const std::string& name, const std::string& expression, double value);
		module::select& where_str(const std::string& name, const std::string& expression, const std::string& value);
		module::select& where_expression(const std::string& expression);
		module::select& where_like(const std::string& name,const std::string& value);
		module::select& table(const std::string& table_name);
		module::select& field(sol::table table);
		module::select& page(uint32 page, uint32 count);
		module::select& limit(uint32 start, uint32 count);
		module::select& orderby(const std::string& field, int sort);
		void clear();
		std::shared_ptr<module::mysql_result> query();
		uint64 count();
		static void regist(sol::state& lua);
	private:
		std::shared_ptr<ylib::select> m_select;
	};
	class update {
	public:
		update(ylib::mysql::conn* conn);
		~update();
		module::update& table(const std::string& table_name);
		module::update& set_i32(const std::string& name, int32 value);
		module::update& set_i64(const std::string& name, int64 value);
		module::update& set_dob(const std::string& name, double value);
		module::update& set_str(const std::string& name, const std::string& value);
		module::update& set(const std::string& expression);
		module::update& where_i32(const std::string& name, const std::string& expression, int32 value);
		module::update& where_i64(const std::string& name, const std::string& expression, int64 value);
		module::update& where_dob(const std::string& name, const std::string& expression, double value);
		module::update& where_str(const std::string& name, const std::string& expression, const std::string& value);
		module::update& where_expression(const std::string& expression);
		module::update& page(uint32 page, uint32 count);
		module::update& limit(uint32 start, uint32 count);
		module::update& orderby(const std::string& field, int sort);
		uint64 exec();
		void clear();
		static void regist(sol::state& lua);
	private:
		std::shared_ptr<ylib::update> m_update;
	};
	class insert {
	public:
		insert(ylib::mysql::conn* conn);
		~insert();
		module::insert& table(const std::string& table_name);
		module::insert& set_i32(const std::string& name, int32 value);
		module::insert& set_i64(const std::string& name, int64 value);
		module::insert& set_dob(const std::string& name, double value);
		module::insert& set_str(const std::string& name, const std::string& value);
		module::insert& set_not_ppst(const std::string& name, const std::string& value);
		uint64 exec();
		void clear();
		static void regist(sol::state& lua);
	private:
		std::shared_ptr<ylib::insert> m_insert;
	};
	class delete_ {
	public:
		delete_(ylib::mysql::conn* conn);
		~delete_();
		module::delete_& table(const std::string& table_name);
		module::delete_& where_i32(const std::string& name, const std::string& expression, int32 value);
		module::delete_& where_i64(const std::string& name, const std::string& expression, int64 value);
		module::delete_& where_dob(const std::string& name, const std::string& expression, double value);
		module::delete_& where_str(const std::string& name, const std::string& expression, const std::string& value);
		module::delete_& where_expression(const std::string& expression);
		module::delete_& page(uint32 page, uint32 count);
		module::delete_& limit(uint32 start, uint32 count);
		module::delete_& orderby(const std::string& field, int sort);
		uint64 exec();
		void clear();
		static void regist(sol::state& lua);
	private:
		std::shared_ptr <ylib::delete_> m_delete;
	};
	/// <summary>
	/// MYSQL连接池
	/// </summary>
	class mysql :public imodule {
	public:
		mysql();
		~mysql();
		/// <summary>
		/// 启动
		/// </summary>
		/// <param name="ipaddress"></param>
		/// <param name="username"></param>
		/// <param name="password"></param>
		/// <param name="database"></param>
		/// <param name="charset"></param>
		/// <param name="port"></param>
		/// <param name="size"></param>
		/// <returns></returns>
		bool start(const std::string& ipaddress, const std::string& username, const std::string& password, const std::string& database, const std::string& charset, ushort port, int32 size);
		/// <summary>
		/// 关闭
		/// </summary>
		void close();

		std::shared_ptr<module::select> select();
		std::shared_ptr<module::insert> insert();
		std::shared_ptr<module::update> update();
		std::shared_ptr<module::delete_> delete_();
	private:
		ylib::mysql::pool* m_pool = nullptr;

		// 通过 imodule 继承
		virtual void regist_global(const std::string& name, sol::state* lua);
	};

}

