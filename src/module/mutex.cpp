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
