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
