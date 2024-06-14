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

#include "response.h"
#include "net/http_reqpack.h"

module::response::response(network::http::response* response) :m_response(response)
{

}

module::response::~response()
{
}

void module::response::regist(sol::state* lua)
{
    // 绑定 Request 类到 Lua
    lua->new_usertype<module::response>("module_response",
        "send_data", &module::response::send_data,
        "send", &module::response::send,
        "send_file", &module::response::send_file,
        "header", &module::response::header,
        "redirect", &module::response::redirect,
        "forward", &module::response::forward
    );
}
bool module::response::send_data(const char* buf, size_t buf_len, ushort stateNum, const std::string& stateDesc)
{
    return m_response->send(buf, buf_len, stateNum, stateDesc);
}
bool module::response::send(const std::string& value)
{
    return m_response->send(value);
}
bool module::response::sendex(const std::string& value, ushort stateNum, const std::string& stateDesc)
{
    return m_response->send(value, stateNum, stateDesc);
}
bool module::response::send_file(const std::string& filepath, int32 downbaud, ushort stateNum, const std::string& stateDesc)
{
    return m_response->send_file(filepath, downbaud, stateNum, stateDesc);
}
bool module::response::redirect(const std::string& filepath, bool MovedPermanently)
{
    return m_response->redirect(filepath, MovedPermanently);
}
bool module::response::forward(const std::string& filepath)
{
    return m_response->forward(filepath);
}
void module::response::header(const std::string& name, const std::string& value)
{
    m_response->headers()->emplace(name, value);
}