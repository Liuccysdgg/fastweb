#pragma once
#include <vector>
#include <string>
#include "base/define.h"
#include "util/ini.h"
#define FASTWEB_VERSION "V1.0.5"
#define FASTWEB_MODULE_JSON_URL "https://download.fwlua.com/module/module.json"
class fastweb
{
public:
    fastweb(const std::vector<std::string>& param);
    ~fastweb();
private:
    struct module_info{
        std::string id;
        std::string name;
        std::string name_en;
        std::string doc;
        std::string desc;
        std::string type;
        std::string download_win64;
        std::string download_linux_type;
        std::string download_linux_url;
    };
private:
    /// @brief 启动
    /// @param ini_filepath 
    /// @param wait
    void start(std::string ini_filepath);
    /// @brief 停止
    /// @param app 
    void stop(void* app);
    /// @brief 输出HELP信息
    void output_help();

    /// @brief 创建配置文件
    /// @param dirpath 
    void create_config(std::string dirpath);
    /// @brief 创建网站木板
    /// @param dirpath 
    void create_website(std::string dirpath);
    /// @brief 安装模块
    /// @param ini_filepath 
    /// @param name 
    void install_module(std::string ini_filepath,std::string name);
    /// @brief 卸载模块
    /// @param ini_filepath 
    /// @param name 
    void uninstall_module(std::string ini_filepath,std::string name);




    /// @brief 模块列表
    void module_list();

    /// @brief 模块信息
    /// @return 
    bool module_infos(std::vector<fastweb::module_info>& list);

private:
    /// @brief 无限等待
    void wait();

    void install_module_linux(fastweb::module_info info);
    void install_module_windows(fastweb::module_info info);



    /// @brief 解析INI配置文件
    /// @param ini_filepath 
    bool parse_config(const std::string& ini_filepath);
private:
    ylib::ini m_ini;
};