#pragma once


#include <string>
#include <fstream>
#include <map>

#include "base/define.h"
#include "base/error.h" 
#include "base/buffer.h"

namespace ylib
{
	namespace file
	{
		/**
		 * @brief 删除文件
		 * @param filepath
		 * @return 
		 */
		bool remove(const std::string& filepath);
		/**
		 * @brief 删除目录
		 * @param dirpath
		 * @param recycle
		 * @return
		 */
		bool remove_dir(const std::string& dirpath
#if _WIN32
			, bool recycle = false
#endif
		);
		/**
		 * @brief 创建目录
		 * @param dirpath
		 * @return
		 */
		bool create_dir(const std::string& dirpath, bool create_parent_dir = true);

		/**
		 * @brief 读取文件
		 * @param filepath
		 * @return
		 */
		ylib::buffer read(const std::string& filepath);
		bool read(const std::string& filepath, ylib::buffer& data);
		/**
		 * @brief 写到文件
		 * @param filepath
		 * @param data
		 * @return
		 */
		bool write(const std::string& filepath, const ylib::buffer& data);
		bool write(const std::string& filepath, const char* data, size_t len);
#ifdef _WIN32
		/**
		 * @brief 列出目录文件
		 * @param rootPath
		 * @param list<std::string,bool[文件夹]>
		 * @return
		 */
		bool list(const std::string& rootPath, std::map<std::string, bool>& list);
#endif
		/**
		 * @brief 取扩展名
		 * @param path
		 * @return
		 */
		std::string ext(const std::string& path);

		/**
		 * @brief 是否存在文件
		 * @param filepath
		 * @return
		 */
		bool exist(const std::string& filepath);
		/**
		 * @brief 是否存在目录
		 * @param filepath
		 * @return
		 */
		bool exist_dir(const std::string& dirpath);

		/**
		 * @brief 取文件大小
		 * @param filepath
		 * @return
		 *      成功>=0 失败=-1
		 */
		int64 size(const std::string& filepath);

		/**
		 * @brief 取上级目录
		 * @param path
		 * @return
		 */
		std::string parent_dir(const std::string& path);

		/**
		 * @brief 取文件名
		 * @param path
		 * @param have_ext
		 * @return
		 */
		std::string filename(const std::string& path, bool have_ext = true);
		/**
		 * @brief 复制文件
		 */
		bool copy(const std::string& src, const std::string& dst);
		/**
		* @brief 复制目录
		*/
		void copy_dir(const std::string& src, const std::string& dst);
		// 遍历目录
		std::map<std::string, ylib::FileType> traverse(const std::string& dirpath, const std::string& regex_pattern = "(.*\\.txt$)");

		// 生成一个临时文件路径
		std::string temp_filepath();

		// 格式化目录
		std::string format_separator(const std::string& filepath);

		// 最后修改时间
		timestamp last_write_time(const std::string& filepath);

	}
	class file_io :public ylib::error_base
	{
	public:
		file_io();
		~file_io();
		bool open(const std::string& filepath, bool only_read = false, bool auto_create = true);
		void close();
		bool appead(const char* data, int64 len);
		bool appead(const ylib::buffer& data);
		bool write(const char* data, int64 len);
		bool write(const ylib::buffer& data);
		ylib::buffer read(int64 size);
		bool read(int64 size, ylib::buffer& data);
		void jump(int64 offset, std::ios_base::seekdir way = std::ios::cur);
		std::streampos cur();
		bool clear();
		std::streamsize size();
		bool is_open();
		inline const std::string& filepath() { return m_filepath; }
	private:
		std::string m_filepath;
		std::fstream* m_stream = nullptr;
		bool m_only_read = false;
	};
}
