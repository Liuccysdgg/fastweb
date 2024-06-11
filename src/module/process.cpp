// MIT License﻿
// Copyright(c) 2024 FastWeb - fwlua.com - nianhua﻿
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :﻿
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.﻿
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.﻿
// ## Additional Terms for Commercial Use﻿
// This software is licensed for personal, educational, and non - commercial use.
// For commercial use or use within a company, organization, or institution, a
// separate commercial license is required.To obtain a commercial license,
// please contact
// EMail：1585346868@qq.com
// Mobile：17367918735
// QQ：1585346868

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

size_t module::process::exist(const std::string& filepath)
{
    return ylib::process::exist(filepath);
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
