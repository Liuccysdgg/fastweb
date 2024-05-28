#pragma once
#include "base/error.h"
#include "mysql.h"
#include <any>
namespace ylib
{
	struct where {
		std::string name;
		std::string expression;
		std::any value;
		// 0=普通 1=Like 2=自定义
		int type = 0;
	};
	struct limit {
		int64 start = -1;
		int64 count = -1;
	};
	enum sort {
		ASC,
		DESC
	};
	struct order_by {
		std::string field;
		ylib::sort sort;
	};
	struct keyvalue {
		std::string name;
		std::any value;
		int extra = 0;
	};
	/// <summary>
	/// MYSQL SELECT 查询器
	/// </summary>
	class select :public ylib::error_base {
	public:
		select(mysql::conn* conn);
		~select();
		/// <summary>
		/// 表名
		/// </summary>
		/// <param name="field"></param>
		/// <returns></returns>
		select& table(const std::string& table_name);
		/// <summary>
		/// 查询字段
		/// </summary>
		/// <param name="field"></param>
		/// <param name="sort"></param>
		/// <returns></returns>
		select& field(const std::vector<std::string>& field);
		/// <summary>
		/// 条件[A=B]
		/// </summary>
		select& where(const std::string& name, const std::string& expression, const std::any& value);
		/// <summary>
		/// 条件[模糊查询]
		/// </summary>
		select& where_like(const std::string& name, const std::string& value);
		/// <summary>
		/// 条件[自定义]
		/// </summary>
		select& where(const std::string& expression);
		/// <summary>
		/// LIMIT[页]
		/// </summary>
		select& page(uint32 page, uint32 count);
		/// <summary>
		/// LIMIT
		/// </summary>
		select& limit(uint32 start, uint32 count);
		/// <summary>
		/// 排序
		/// </summary>
		select& orderby(const std::string& field, sort sort = DESC);
		/// <summary>
		/// 查询数量
		/// </summary>
		/// <returns></returns>
		uint64 count();
		/// <summary>
		/// 查询
		/// </summary>
		ylib::mysql::result* query();
#if 0
		/// <summary>
		/// 查询转换为layui需求模板
		/// </summary>
		/// <returns></returns> 
		ylib::json query_layui();
#endif
	private:
		/// <summary>
		/// 生成SQL片段
		/// </summary>
		/// <param name="field_name"></param>
		/// <param name="where"></param>
		/// <param name="orderby"></param>
		/// <param name="limit"></param>
		void make_sql(std::string& field_name, std::string& where, std::string& orderby, std::string& limit, std::vector<std::any>& insert_values);
	private:
		mysql::conn* m_conn = nullptr;
		std::vector<ylib::where> m_wheres;
		std::string m_table_name;
		std::vector<std::string> m_fields;
		ylib::limit m_limit;
		order_by m_orderby;

	};
	/// <summary>
	/// MYSQL UPDATE
	/// </summary>
	class update :public ylib::error_base {
		struct set {
			std::string name;
			std::any value;
			std::string expression;
			// 0=普通 1=表达式
			int type = 0;
		};
	public:
		update(mysql::conn* conn);
		~update();
		/// <summary>
		/// 表名
		/// </summary>
		/// <param name="field"></param>
		/// <returns></returns>
		update& table(const std::string& table_name);
		/// <summary>
		/// 更新
		/// </summary>
		update& set(const std::string& name, const std::any& value);
		update& set(const std::string& expression);
		/// <summary>
		/// 条件[A=B]
		/// </summary>
		update& where(const std::string& name, const std::string& expression, const std::any& value);
		/// <summary>
		/// 条件[模糊查询]
		/// </summary>
		update& where_like(const std::string& name, const std::string& value);
		/// <summary>
		/// 条件[自定义]
		/// </summary>
		update& where(const std::string& expression);
		/// <summary>
		/// LIMIT[页]
		/// </summary>
		update& page(uint32 page, uint32 count);
		/// <summary>
		/// LIMIT
		/// </summary>
		update& limit(uint32 start, uint32 count);
		/// <summary>
		/// 排序
		/// </summary>
		update& orderby(const std::string& field, sort sort = DESC);
		/// <summary>
		/// 查询
		/// </summary>
		uint64 exec();
	private:
		/// <summary>
		/// 生成SQL片段
		/// </summary>
		/// <param name="set"></param>
		/// <param name="where"></param>
		/// <param name="orderby"></param>
		/// <param name="limit"></param>
		void make_sql(std::string& set, std::string& where, std::string& orderby, std::string& limit, std::vector<std::any>& insert_values);
	private:
		mysql::conn* m_conn = nullptr;
		std::vector<ylib::where> m_wheres;
		std::string m_table_name;
		std::vector<struct ylib::update::set> m_sets;
		ylib::limit m_limit;
		order_by m_orderby;

	};
	/// <summary>
	/// MYSQL INSERT
	/// </summary>
	class insert :public ylib::error_base {
	public:
		insert(mysql::conn* conn);
		~insert();
		/// <summary>
		/// 表名
		/// </summary>
		/// <param name="field"></param>
		/// <returns></returns>
		insert& table(const std::string& table_name);
		/// <summary>
		/// 更新
		/// </summary>
		insert& set(const std::string& name, const std::any& value);
		insert& set_not_pret(const std::string& name, const std::string& value);
		/// <summary> 
		/// 查询
		/// </summary>
		uint64 exec();
	private:
		mysql::conn* m_conn = nullptr;
		std::string m_table_name;
		std::vector<keyvalue> m_sets;
	};
	/// <summary>
	/// MYSQL DELETE
	/// </summary>
	class delete_ :public ylib::error_base {
	public:
		delete_(mysql::conn* conn);
		~delete_();
		/// <summary>
		/// 表名
		/// </summary>
		/// <param name="field"></param>
		/// <returns></returns>
		delete_& table(const std::string& table_name);
		/// <summary>
		/// 条件[A=B]
		/// </summary>
		delete_& where(const std::string& name, const std::string& expression, const std::any& value);
		/// <summary>
		/// 条件[模糊查询]
		/// </summary>
		delete_& where_like(const std::string& name, const std::string& value);
		/// <summary>
		/// 条件[自定义]
		/// </summary>
		delete_& where(const std::string& expression);
		/// <summary>
		/// LIMIT[页]
		/// </summary>
		delete_& page(uint32 page, uint32 count);
		/// <summary>
		/// LIMIT
		/// </summary>
		delete_& limit(uint32 start, uint32 count);
		/// <summary>
		/// 排序
		/// </summary>
		delete_& orderby(const std::string& field, sort sort = DESC);
		/// <summary>
		/// 查询
		/// </summary>
		uint64 exec();
	private:
		/// <summary>
		/// 生成SQL片段
		/// </summary>
		/// <param name="where"></param>
		/// <param name="orderby"></param>
		/// <param name="limit"></param>
		void make_sql(std::string& where, std::string& orderby, std::string& limit, std::vector<std::any>& insert_values);
	private:
		mysql::conn* m_conn = nullptr;
		std::vector<ylib::where> m_wheres;
		std::string m_table_name;
		ylib::limit m_limit;
		order_by m_orderby;

	};
}
