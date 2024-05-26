#pragma once
#include <string>
#include <vector>
#include "util/json.h"
#include "util/counter.hpp"
namespace leveldb
{
	class DB;
}
namespace ylib
{
	/// <summary>
	/// 本地存储(LevelDB)
	/// </summary>
	class local_storage
	{
	public: 
		local_storage();
        ~local_storage();
        /// <summary>
        /// 打开
        /// </summary>
        /// <param name="dirpath">目录</param>
        /// <returns></returns>
        bool open(const std::string& dirpath);
		/// <summary>
		/// 关闭
		/// </summary>
		void close();
		/// <summary>
		/// 清空数据库
		/// </summary>
		void clear();
		/// <summary>
		/// 写
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <returns></returns>
		bool write(const std::string& name,const std::string& value);
		/// <summary>
		/// 读
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <returns></returns>
		bool read(const std::string& name, std::string& value);
		/// <summary>
		/// 删除
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool del(const std::string& name);
		/// <summary>
		/// 是否存在
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool exist(const std::string& name);


		/// <summary>
		/// [测试] 生成GUID
		/// </summary>
		/// <returns></returns>
		std::string test_make_guid();
	private:
		// 目录
        std::string m_dirpath;
		// DB指针
		leveldb::DB* m_db = nullptr;
		// GUID索引器
		ylib::counter<uint32> m_guid_counter;
	};
}


