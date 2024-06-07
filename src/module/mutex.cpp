#include "mutex.h"

module::mutex::mutex()
{
}

module::mutex::~mutex()
{
}

void module::mutex::lock()
{
    m_mutex.lock();
}

void module::mutex::unlock()
{
    m_mutex.unlock();
}

bool module::mutex::try_lock()
{
    return m_mutex.try_lock();
}

void module::mutex::regist(sol::state* lua)
{
    lua->new_usertype<module::mutex>("mutex",
        "new", sol::constructors<module::mutex()>(),
        "lock", &module::mutex::lock,
        "unlock", &module::mutex::unlock,
        "try_lock", &module::mutex::try_lock,
        "self", &module::mutex::self
    );
}

void module::mutex::regist_global(const char* name, sol::state* lua)
{
    lua->registry()[name] = this;
    (*lua)[name] = this;
}

module::auto_lock::auto_lock(module::mutex& mutex):m_mutex(&mutex)
{
    m_mutex->lock();
}

module::auto_lock::~auto_lock()
{
    m_mutex->unlock();
}

void module::auto_lock::regist(sol::state* lua)
{
    lua->new_usertype<module::auto_lock>("auto_lock",
        "new", sol::constructors<module::auto_lock(module::mutex&)>()
    );
}
