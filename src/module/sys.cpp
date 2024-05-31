#include "sys.h"
#include "util/system.h"
std::string module::sys::current_dir()
{
    return system::current_dir();
}
void module::sys::regist(sol::state* lua)
{
    lua->new_usertype<module::sys>("sys",
        "current_dir", &module::sys::current_dir
    );
}
