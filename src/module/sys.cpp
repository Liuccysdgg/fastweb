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
        "mac", &module::sys::mac,
    );
}
