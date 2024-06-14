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

// QQ：1585346868

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
