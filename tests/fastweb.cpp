/*Software License

Copyright(C) 2024[liuyingjie]
License Terms
Usage Rights

Any individual or entity is free to use, copy, and distribute the binary form of this software without modification to the source code, without the need to disclose the source code.
If the source code is modified, the modifications must be open - sourced under the same license.This means that the modifications must be disclosed and accompanied by a copy of this license.
Future Versions Updates
From this version onwards, all future releases will be governed by the terms of the latest version of the license.This license will automatically be nullified and replaced by the new version.
Users must comply with the terms of the new license issued in future releases.
Liability and Disclaimer
This software is provided “as is”, without any express or implied warranties, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non - infringement.In no event shall the author or copyright holder be liable for any claims, damages, or other liabilities, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
Contact Information
If you have any questions, please contact us: 1585346868@qq.com Or visit our website fwlua.com.
*/
#include "fastweb.h"
#include <filesystem>
#include "util/strutils.h"
#include "util/system.h"
#include "util/codec.h"
#include "util/file.h"
#include "util/print.h"
#include "../src/utils/parseconfig.hpp"
#include "net/http_client_plus.h"
#include "core/entry.h"




bool exsit_install_software(std::string name){
    std::string command = "which " + name + " > /dev/null 2>&1";
    int result = std::system(command.c_str());
    return (result == 0);
};
std::pair<int,std::string> execute_command(const std::string& command){
    std::array<char, 128> buffer;
    std::string result;
    int return_code = 0;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((command + " 2>&1").c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read the output a line at a time - output it.
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // Get the return code
    return_code = pclose(pipe.release());
    
    return std::make_pair(return_code, command+"\n"+result);
};





fastweb::fastweb(const std::vector<std::string> &param)
{
    
    auto equals=[&](std::vector<std::string> p2,int all_size)->bool{
        if(param.size() < all_size)
            return false;
        if(param.size()<p2.size())
            return false;
        for(size_t i=0;i<p2.size();i++)
        {
            if(param[i] != p2[i])
                return false;
        }
        return true;
    };
    

    std::cout << "|------------------------[FASTWEB]----------------------------" << std::endl;
	std::cout << "| 软件版本:\t"<< FASTWEB_VERSION << std::endl;
	std::cout << "| 工作目录:\t"<< strutils::replace(system::current_dir(),'\\','/') << std::endl;
#ifdef _WIN32
    std::cout << "| Windows平台推荐使用网站管理器部署：https://fwlua.com/thread-3-1-1.html"<<std::endl;
#endif
	std::cout << "| 启动参数" << std::endl;
    
	for (int i = 0; i < param.size(); i++)
	{
		std::cout << "|\t" << param[i] << std::endl;
	}
	std::cout << "|----------------------------------------------------------------" << std::endl;


    if(equals({"start"},2))
    {
        start(param[1]);
    }
    else if(equals({"start2"},2))
    {
        start(param[1]);
    }
    else if(equals({"create","config"},3))
    {
        create_config(param[2]);
    }
    else if(equals({"create","website"},3))
    {
        create_website(param[2]);
    }
    else if(equals({"instmod"},3))
    {
        install_module(param[1],param[2]);
    }
    else if(equals({"uninstmod"},3))
    {
        uninstall_module(param[1],param[2]);
    }
    else if(equals({"module","list"},2))
    {
        module_list();
    }
    else
    {
        output_help();
    }
}

fastweb::~fastweb()
{
}

void fastweb::start(std::string ini_filepath)
{
    ini_filepath = std::filesystem::absolute(ini_filepath).string();

    if (ylib::file::exist(ini_filepath))
    {
        if (fastweb_start(ini_filepath.c_str()) == nullptr)
            return;
        wait();
    }
    else
    {
        std::cerr << "未找到配置文件: " << ini_filepath<< std::endl;
    }
}

void fastweb::stop(void *app)
{
    if(app != nullptr)
    {
        fastweb_close(app);
    }
}

void fastweb::output_help()
{
    int index = 1;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
	std::cout << "| Fastweb指令说明:" << std::endl;
	std::cout << "|"<<index++<<"、fastweb start config.ini\t\t [加载配置文件并启动Fastweb]" << std::endl;
#ifndef _WIN32
	std::cout << "|"<<index++<<"、fastweb create config .\t\t [创建配置文件模板到指定路径]" << std::endl;
	std::cout << "|"<<index++<<"、fastweb create website .\t\t [创建网站示例模板到指定路径]" << std::endl;
#endif
	std::cout << "|"<<index++<<"、fastweb instmod config.ini mysql\t\t [为指定项目安装模块]" << std::endl;
    std::cout << "|"<<index++<<"、fastweb uninstmod config.ini mysql\t\t [为指定项目卸载模块]" << std::endl;
	std::cout << "|"<<index++<<"、fastweb module list\t\t [显示所有模块列表]" << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
}

void fastweb::create_config(std::string dirpath)
{
    ylib::file::copy("/usr/local/share/fastweb/config.ini",dirpath);
}

void fastweb::create_website(std::string dirpath)
{
    ylib::file::copy_dir("/usr/local/share/fastweb", dirpath);
}

void fastweb::install_module(std::string ini_filepath, std::string name)
{
    if(parse_config(ini_filepath) == false)
        return;
    std::vector<fastweb::module_info> list;
    if(module_infos(list) == false)
        return;

    fastweb::module_info info;
    for(size_t i=0;i<list.size();i++)
    {
        if(list[i].name_en == name)
        {
            info = list[i];
            break;
        }
    }
    if(info.name == "")
    {
        std::cout<<"未找到 `"<<name<<"` 模块"<<std::endl;
        return;
    }
#ifdef _WIN32
    install_module_windows(info);
#else
    install_module_linux(info);
#endif

    
}

void fastweb::uninstall_module(std::string ini_filepath, std::string name)
{
    if(parse_config(ini_filepath) == false)
        return;

    
    std::vector<fastweb::module_info> list;
    if(module_infos(list) == false)
        return;

    fastweb::module_info info;
    for(size_t i=0;i<list.size();i++)
    {
        if(list[i].name_en == name)
        {
            info = list[i];
            break;
        }
    }
    if(info.name == "")
    {
        std::cout<<"未找到 `"<<name<<"` 模块"<<std::endl;
        return;
    }

    std::string module_dirpath = m_ini.read("scripts","module_dir")+"/.install/"+info.id;
    if(ylib::file::exist_dir(module_dirpath))
    {
        if(ylib::file::remove_dir(module_dirpath) == false)
        {
            std::cerr<<"删除模块目录失败("+module_dirpath+")"<<std::endl;
        }
        else
        {
            std::cerr<<"删除模块目录成功("+module_dirpath+")"<<std::endl;
        }
    }
#ifndef _WIN32
    if(info.type == "/opt/lua54/bin/lua")
    {
        std::cout<<"正在卸载 `"<<info.name_en<<"` 库"<<std::endl;
        auto [code,err] = execute_command("sudo luarocks remove "+info.name_en);
        if(code == 0)
        {
            std::cout<<"卸载成功"<<std::endl;
        }
        else
        {
            std::cout<<"卸载失败:"<<std::endl;
            std::cout<<err<<std::endl;
        }
    }
#endif
    std::cout<<"OK"<<std::endl;
}

void fastweb::module_list()
{
    std::vector<fastweb::module_info> list;
    if(module_infos(list) == false)
        return;

    std::cout<<"--------------------------模块列表---------------------------"<<std::endl;
    for(size_t i=0;i<list.size();i++)
    {
#ifdef _WIN32
        if(list[i].download_win64.empty())
            continue;
#else
        if(list[i].download_linux_type.empty())
            continue;
#endif
        if(i!=0)
            std::cout<<"|-------------------------------------------------------------"<<std::endl;
        std::cout<<"| 模块名称:\t"<<list[i].name_en<<std::endl;
        std::cout<<"| 中文名称:\t"<<list[i].name<<std::endl;
        std::cout<<"| 描述信息:\t"<<list[i].desc<<std::endl;
        std::cout<<"| 文档地址:\t"<<list[i].doc<<std::endl;
        std::cout<<"| 模块类型:\t";
        if(list[i].type == "fastweb")
            std::cout<<"FastWeb"<<std::endl;
        else if(list[i].type == "lua")
            std::cout<<"LUA"<<std::endl;
        else if(list[i].type == "other")
            std::cout<<"网友上传"<<std::endl;
    }
    std::cout<<"|------------------------------------------------------------"<<std::endl;
}

bool fastweb::module_infos(std::vector<fastweb::module_info>& list)
{
    std::cout<<"正在获取最新模块列表信息..."<<std::endl;
    network::http::client_plus client;
    if(client.get(FASTWEB_MODULE_JSON_URL) == false)
    {
        std::cerr<<"下载模块列表信息失败: "<<FASTWEB_MODULE_JSON_URL<<", "<<client.last_error()<<std::endl;
        return false;
    }
    if(client.status() != 200)
    {
        std::cerr<<"下载模块列表信息失败: "<<FASTWEB_MODULE_JSON_URL<<", status: "<<client.status()<<std::endl;
        return false;
    }
    auto data = ylib::json::from(client.response());

    std::vector<std::string> types = {"fastweb","lua","other"};
    for(size_t i=0;i<types.size();i++)
    {
        auto td = data[types[i]];
        for(size_t k = 0;k<td.size();k++)
        {
            module_info info;
            info.id = std::to_string(td[k]["id"].to<int>());
            info.name = td[k]["name"].to<std::string>();
            info.name_en = td[k]["name_en"].to<std::string>();
            info.desc = td[k]["desc"].to<std::string>();
            info.doc = td[k]["doc"].to<std::string>();
            info.type = types[i];
            info.download_win64 = td[k]["doc"].to<std::string>();
            info.download_linux_url = td[k]["download"]["linux"]["url"].to<std::string>();
            info.download_linux_type = td[k]["download"]["linux"]["type"].to<std::string>();

            list.push_back(info);
        }
    }
    return true;
}

void fastweb::wait()
{
    while(true)
        system::sleep_msec(1000*60);
}

void fastweb::install_module_linux(fastweb::module_info info)
{
  
    
    if(exsit_install_software("/opt/lua54/bin/lua") == false)
    {
        std::cout<<"LUA未安装，请重新执行fastweb一键构建脚本并确认lua编译安装部分正确执行"<<std::endl;
        return;
    }
    if(info.type == "fastweb" || info.type == "other")
    {
        if(info.download_linux_type == "zip")
        {

            std::cout<<"正在安装 `"<<info.name_en<<"` 库"<<std::endl;
            network::http::client_plus client;
            if(client.get(info.download_linux_url) == false)
            {
                std::cerr<<"下载模块压缩包失败: "<<info.download_linux_url<<", "<<client.last_error()<<std::endl;
                return;
            }
            if(client.status() != 200)
            {
                std::cerr<<"下载模块压缩包失败: "<<info.download_linux_url<<", status: "<<client.status()<<std::endl;
                return;
            }
            std::string zip_filepath = ylib::file::temp_filepath()+".zip";
            ylib:file::write(zip_filepath,client.response());

            
            std::string new_module_dirpath = m_ini.read("scripts","module_dir")+"/.install/"+info.id;
            ylib::file::create_dir(new_module_dirpath,true);
            std::string cmd = "unzip -o -q "+zip_filepath+" -d "+new_module_dirpath;
            
            auto [code,err] = execute_command(cmd);
            if(code != 0)
            {
                std::cerr<<"安装失败："<<std::endl;
                std::cerr<<cmd<<std::endl;
                std::cerr<<err<<std::endl;
            }
            else
            {
                std::cerr<<"安装成功"<<std::endl;
            }
        }
        else if(info.download_linux_type == "github")
        {
            std::cout<<"正在安装 `"<<info.name_en<<"` 库"<<std::endl;
            std::string tdir = ylib::file::temp_filepath();
            ylib::file::create_dir(tdir,true);
            std::string cmd = "git clone "+info.download_linux_url+" "+tdir;
            auto [code,result] = execute_command(cmd);
            if(code != 0)
            {
                std::cerr<<"克隆仓库失败："<<std::endl;
                std::cerr << cmd << std::endl;
                std::cerr << result << std::endl;
                return;
            }
            cmd = tdir+"/build.sh";
            auto [code2,result2] = execute_command(cmd);
            if(code2 != 0)
            {
                std::cerr<<"执行安装命令失败："<<std::endl;
                std::cerr << cmd << std::endl;
                std::cerr << result2 << std::endl;
                return;
            }
            
            std::string new_module_dirpath = m_ini.read("scripts","module_dir")+"/.install/"+info.id;
            ylib::file::create_dir(new_module_dirpath,true);
            ylib::file::copy_dir(tdir+"/target",new_module_dirpath);
            std::cout<<"安装成功"<<std::endl;
        }
    }
    else if(info.type == "lua")
    {
        if(exsit_install_software("luarocks") == false)
        {
            std::cout<<"`luarocks` 未安装，请先执行以下命令安装：sudo apt install luarocks"<<std::endl;
            return;
        }
        std::cout<<"正在安装 `"<<info.name_en<<"` 库"<<std::endl;
        auto [code,err] = execute_command("sudo luarocks install "+info.name_en+" LUA_INCDIR=/opt/lua54/include LUA_LIBDIR=/opt/lua54/lib");
        if(code == 0)
        {
            std::cout<<"安装成功"<<std::endl;
        }
        else
        {
            std::cout<<"安装失败:"<<std::endl;
            std::cout<<err<<std::endl;
        }
    }
    
}

void fastweb::install_module_windows(fastweb::module_info info)
{
}

bool fastweb::parse_config(const std::string &ini_filepath)
{
    auto [result,err] = parseconfig(m_ini,ini_filepath);
    if(result == false)
    {
        std::cerr<<err<<std::endl;
        return false;
    }
    return true;
}
