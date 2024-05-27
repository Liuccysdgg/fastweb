#include "time.h"
#include "util/time.h"
uint64 module::time::now_msec()
{
    return ylib::time::now_msec();
}
uint64 module::time::now_sec()
{
    return ylib::time::now_sec();
}

void module::time::regist(sol::state* lua)
{
    lua->new_usertype<module::time>("time",
        "now_msec", &module::time::now_msec,
        "now_sec", &module::time::now_sec
    );
}
