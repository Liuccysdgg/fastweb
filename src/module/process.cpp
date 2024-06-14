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

#include "process.h"
#include "util/process.h"
#ifdef _WIN32
#include <Windows.h>
#endif
size_t module::process::create(const std::string& filepath, const std::string& working_directory, sol::table args, bool wait_close, bool show_window)
{
    std::vector<std::string> vs_args;
    for (auto& pair : args) {
        if (pair.second.get_type() != sol::type::string)
        {
            throw ylib::exception("`args ` only supports string type");
        }
        vs_args.push_back(pair.second.as<std::string>());
    }

    size_t pid = 0;
    if (ylib::process::create(filepath, working_directory, vs_args, wait_close, show_window,nullptr,&pid) == false)
        return 0;
    return pid;
}

bool module::process::destory(size_t pid)
{
    return ylib::process::destory(pid);
}

sol::table module::process::list(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    int row_num = 1;
    auto pl = ylib::process::list();
    for_iter(iter, pl)
    {
        sol::table row = lua.create_table();
        row["name"] = iter->name;
        row["parent_pid"] = iter->parent_pid;
        row["path"] = iter->path();
        row["pid"] = iter->pid;
        result_table[row_num++] = row;
    }
    return result_table;
}

sol::table module::process::exist(const std::string& filepath, sol::this_state s)
{
    auto list = ylib::process::exist(filepath);
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    for (size_t i = 0; i < list.size(); i++)
        result_table[i + 1] = list[i];

    return result_table;
}

bool module::process::exist_pid(size_t pid)
{
#ifdef _WIN32
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess != nullptr) {
        // 进程存在，关闭句柄并返回true
        CloseHandle(hProcess);
        return true;
    }
    // 进程不存在
    return false;
#else
    return false;
#endif
}

std::string module::process::getpath(size_t pid)
{
    return ylib::process::getpath(pid);
}

void module::process::regist(sol::state* lua)
{
    lua->new_usertype<module::process>("process",
        "create", &module::process::create,
        "destory", &module::process::destory,
        "exist", &module::process::exist,
        "exist_pid", &module::process::exist_pid,
        "getpath", &module::process::getpath,
        "list", &module::process::list
    );
}
