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
    lua->new_usertype<module::response>("fw_response",
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