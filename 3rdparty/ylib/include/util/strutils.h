#pragma once
#include <string>
#include <vector>
#include "base/define.h"
namespace ylib
{
    namespace strutils{
        /** 
         * @brief 删除子字符串
         * @param str
         * @param sub
         * @return
         */
        std::string remove(std::string str, const std::string& sub);
        /**
         * @brief 大小写转换
         * @param input
         * @param toUpperCase
         * @return
         */
        std::string change_case(const std::string& input, bool toUpperCase/*true=大写,false=小写*/);
        /**
         * @brief 分割字符串
         * @param str
         * @param delim
         * @return
         */
        std::vector<std::string> split(const std::string& str, const std::string& delim);
        std::vector<std::string> split(const std::string& str,char delim);

        /**
         * @brief 去首尾字符
         * @param str
         * @param chars
         * @return
         */
        std::string trim(std::string s,const std::vector<char>& trim_chars);
        /**
         * @brief 去首字符
         * @param str
         * @param chars
         * @return
         */
        std::string trim_begin(std::string s,const std::vector<char>& trim_chars);
        /**
         * @brief 去尾字符
         * @param str
         * @param chars
         * @param loop = 循环匹配
         * @return
         */
        std::string trim_end(std::string s,const std::vector<char>& trim_chars,bool loop = false);

        /**
         * @brief 左边
         * @param str
         * @param len
         * @return
         */
        std::string left(const std::string& str,size_t len);
        /**
         * @brief 右边
         * @param str
         * @param len
         * @return
         */
        std::string right(const std::string& str,size_t len);

        /**
         * @brief 是否为数字
         * @return 
         */
        bool is_num(const std::string& str);
        bool is_num_code(char value);
        /**
         * @brief 是否为英文
         * @return
         */
        bool is_en_code(char value);
        /**
         * @brief 取中间
         * @param str
         * @param begin
         * @param end
         * @return
         */
        std::string between(const std::string& str,const std::string& begin,const std::string& end);
        /**
         * @brief 替换
         * @param str
         * @param search
         * @param value
         * @return
         */ 
        std::string replace(const std::string& str, const std::string& search, const std::string& value);
        std::string replace(const std::string& str, uchar search,uchar value);
        /**
         * @brief 空值
         */
        std::string F(const char* value);
        
#ifdef _WIN32
        std::wstring to_wstring(const std::string& value);
        std::string wto_string(const std::wstring& value);
#endif 
        /// <summary>
        /// 填补
        /// </summary>
        /// <param name="value">输入</param>
        /// <param name="fixed_length">固定长度</param>
        /// <param name="pad">填补字符</param>
        /// <returns></returns>
        std::string pad_with_begin(const std::string& value,size_t fixed_length,char pad);
        std::string pad_with_end(const std::string& value, size_t fixed_length, char pad);

    }
}
