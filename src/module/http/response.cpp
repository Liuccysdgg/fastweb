#include "response.h"
#include "net/http_reqpack.h"

module::response::response(network::http::response* response) :m_response(response)
{

}

module::response::~response()
{
}

void module::response::regist(sol::state& state)
{
    // 绑定 Request 类到 Lua
    state.new_usertype<module::response>("module_response",
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