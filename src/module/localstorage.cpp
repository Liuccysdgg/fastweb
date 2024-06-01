#include "localstorage.h"
#include "util/localstorage.h"
module::local_storage::local_storage()
{
}

module::local_storage::~local_storage()
{
    ::ylib::local_storage::close();
}

sol::optional<std::string> module::local_storage::readex(const std::string& name)
{
    std::string value;
    if (this->read(name, value) == false)
        return sol::nullopt;
    return value;
}

void module::local_storage::regist(sol::state* lua)
{
    lua->new_usertype<module::local_storage>("local_storage",
        "new", sol::constructors<module::local_storage()>(),
        "clear", &module::local_storage::clear,
        "close", &module::local_storage::close,
        "del", &module::local_storage::del,
        "exist", &module::local_storage::exist,
        "open", &module::local_storage::open,
        "read", &module::local_storage::readex,
        "write", &module::local_storage::write,
        "self", &module::local_storage::self,
        "last_error", &module::local_storage::last_error
    );
}

void module::local_storage::regist_global(const std::string& name, sol::state* lua)
{
    lua->registry()[name] = this;
    (*lua)[name] = this;
}
