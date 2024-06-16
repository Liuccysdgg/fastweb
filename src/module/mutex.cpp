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
    lua->new_usertype<module::mutex>("fw_mutex",
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
    lua->new_usertype<module::auto_lock>("fw_auto_lock",
        "new", sol::constructors<module::auto_lock(module::mutex&)>()
    );
}
