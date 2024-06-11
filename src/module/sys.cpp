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

#include "sys.h"
#include "util/system.h"
std::string module::sys::current_dir()
{
    return system::current_dir();
}
void module::sys::sleep_msec(uint32 msec)
{
    system::sleep_msec(msec);
}
int64 module::sys::random(int64 min, int64 max)
{
    return system::random(min, max);
}
std::string module::sys::current_filepath()
{
    return system::current_filepath();
}
std::string module::sys::temp_path()
{
    return system::temp_path();
}
std::string module::sys::desktop_path()
{
    return system::desktop_path();
}
std::string module::sys::currentuser_path()
{
    return system::currentuser_path();
}
sol::table module::sys::mac(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    auto macs = system::mac();
    for(size_t i=0;i<macs.size();i++)
        result_table[i+1] = macs[i];
    return result_table;
}
void module::sys::regist(sol::state* lua)
{
    lua->new_usertype<module::sys>("sys",
        "current_dir", &module::sys::current_dir,
        "sleep_msec", &module::sys::sleep_msec,
        "random", &module::sys::random,
        "current_filepath", &module::sys::current_filepath,
        "temp_path", &module::sys::temp_path,
        "desktop_path", &module::sys::desktop_path,
        "currentuser_path", &module::sys::currentuser_path,
        "mac", &module::sys::mac
    );
}
