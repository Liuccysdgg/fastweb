#include "config.h"
#include <regex>
bool config::open(const std::string& ini_filepath)
{
	if (ylib::file::exist(ini_filepath) == false)
	{
		LOG_ERROR("not found file: " + ini_filepath);
		return false;
	}
	std::string temp_filepath =  ylib::file::temp_filepath() + ".bak";
	if (ylib::file::copy(ini_filepath, temp_filepath) == false)
	{
		LOG_ERROR("Failed to copy temporary INI configuration file from '" + ini_filepath + "' to '" + temp_filepath + "'.");
		return false;
	}
	std::string src_content = ylib::file::read(temp_filepath);
	if (m_ini.open(temp_filepath))
	{
		auto vars = extractVariableNames(ylib::file::read(temp_filepath));
		for (size_t i = 0; i < vars.size(); i++)
		{
			if (m_ini.read("variable", vars[i]) == "")
			{
				LOG_ERROR("Variable declaration not found or empty content: ${" + vars[i] + "}");
				return false;
			}
			src_content = strutils::replace(src_content, "${" + vars[i] + "}", m_ini.read("variable", vars[i]));
		}
	}
	ylib::file::write(temp_filepath,src_content);
	m_ini.close();
	m_ini.open(temp_filepath);
	cache();
	return true;
}
std::vector<std::string> config::extractVariableNames(const std::string& text)
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

void config::cache()
{
	server.address =  m_ini.read("server", "address");
	server.port = ylib::stoi(m_ini.read("server", "port"));

	scripts.app_dir = m_ini.read("scripts","app_dir");
	scripts.lib_dir = m_ini.read("scripts", "lib_dir");
	scripts.lua_cache_size = ylib::stoi(m_ini.read("scripts", "lua_cache_size"));
	scripts.app_mapping_dir = m_ini.read("scripts", "app_mapping_dir");
	scripts.auto_update_sec = ylib::stoi(m_ini.read("scripts", "auto_update_sec"));

	website.static_dir = m_ini.read("website","static_dir");
	website.default_404 = m_ini.read("website", "default_404");
	website.default_index = strutils::split(m_ini.read("website", "default_index"), ',');
	website.session_dir = m_ini.read("website", "session_dir");
	website.session_timeout_sec = ylib::stoi(m_ini.read("website", "session_timeout_sec"));
	website.Initialization_script = m_ini.read("website", "Initialization_script");

	{
		ylib::json interceptors = ylib::json::from(m_ini.read("website", "interceptor_scripts"));
		for (size_t i = 0; i < interceptors.size(); i++)
		{
			if (interceptors[i].size() != 2)
			{
				LOG_WARN("Interceptor configuration, if the "+std::to_string(i) + "th array has non 2 members, this interceptor will be invalidated.");
				continue;
			}
			website::__interceptor inr;
			inr.filepath = interceptors[i][0].to<std::string>();
			inr.regex_express = interceptors[i][1].to<std::string>();
			website.interceptor_scripts.push_back(inr);
		}
	}
}
