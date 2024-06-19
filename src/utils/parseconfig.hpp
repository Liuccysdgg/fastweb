#pragma once
#include <vector>
#include <regex>
#include <string>
#include <tuple>
#include "util/ini.h"
#include "util/file.h"
#include "util/system.h"
#include "util/strutils.h"
std::vector<std::string> __extractVariableNames(const std::string& text)
{
	std::regex pattern("\\$\\{([^}]+)\\}"); // 使用捕获组提取中间的内容
	std::vector<std::string> results;
	// 使用 std::sregex_iterator 迭代所有匹配项
	auto begin = std::sregex_iterator(text.begin(), text.end(), pattern);
	auto end = std::sregex_iterator();
	for (std::sregex_iterator i = begin; i != end; ++i) {
		std::smatch match = *i;
		results.push_back(match[1]); // 仅添加捕获组的内容
	}
	return results;
}
std::tuple<bool,std::string> parseconfig(ylib::ini &ini,const std::string& ini_filepath)
{
	if (ylib::file::exist(ini_filepath) == false)
	{
        #ifdef FASTWEB_EXE
		    return std::make_tuple(false,"未找到配置文件: " + ini_filepath);
        #else
            return std::make_tuple(false,"not found ini: " + ini_filepath);
        #endif
	}
	std::string temp_filepath =  ylib::file::temp_filepath() + ".bak";
	if (ylib::file::copy(ini_filepath, temp_filepath) == false)
	{
    #ifdef FASTWEB_EXE
		return std::make_tuple(false,"Failed to copy temporary INI configuration file from '" + ini_filepath + "' to '" + temp_filepath + "'.");
    #else
        return std::make_tuple(false,"配置文件复制到临时目录失败 from '" + ini_filepath + "' to '" + temp_filepath + "'.");
    #endif
	}
	std::string src_content = ylib::file::read(temp_filepath);
	// EXE运行目录
	src_content = strutils::replace(src_content, "${current_dir}", strutils::replace(system::current_dir(),'\\','/'));
	// 配置文件目录
	{
		std::string ini_dir = ylib::file::parent_dir(ini_filepath);
		src_content = strutils::replace(strutils::replace(src_content, "${config_dir}", ini_dir), '\\', '/');
	}
	


	ylib::file::write(temp_filepath,src_content);
	if (ini.open(temp_filepath))
	{
		auto vars = __extractVariableNames(ylib::file::read(temp_filepath));
		for (size_t i = 0; i < vars.size(); i++)
		{
			if (ini.read("variable", vars[i]) == "")
			{
				return std::make_tuple(false,"Variable declaration not found or empty content: ${" + vars[i] + "}");
			}
			src_content = strutils::replace(src_content, "${" + vars[i] + "}", ini.read("variable", vars[i]));
		}
	}
	ylib::file::write(temp_filepath,src_content);
	ini.close();
	ini.open(temp_filepath);

    return std::make_tuple(true,"");
}