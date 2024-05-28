#include "codec.h"
#include "util/codec.h"
std::string module::codec::url_de(const std::string& value)
{
    return ylib::codec::url::de(value);
}

std::string module::codec::url_en(const std::string& value)
{
    return ylib::codec::url::en(value);
}

std::string module::codec::to_utf8(const std::string& value)
{
    return ylib::codec::to_utf8(value);
}

std::string module::codec::to_gbk(const std::string& value)
{
    return ylib::codec::to_gbk(value);
}

std::string module::codec::md5(const std::string& value)
{
    return ylib::codec::md5(value);
}

void module::codec::regist(sol::state* lua)
{
    lua->new_usertype<module::codec>("codec",
        "url_de", &module::codec::url_de,
        "url_en", &module::codec::url_en,
        "to_utf8", &module::codec::to_utf8,
        "to_gbk", &module::codec::to_gbk,
        "md5", &module::codec::md5
    );
}
