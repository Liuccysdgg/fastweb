#include "request.h"
#include "net/http_reqpack.h"
#include "session.h"
module::request::request(network::http::request* request) :m_request(request)
{
}
module::request::~request()
{
    if (m_session != nullptr)
        delete m_session;
}
std::string module::request::token()
{
    return m_request->token();
}
module::session* module::request::session(const std::string& token)
{
    if (m_session == nullptr)
        m_session = new module::session(&m_request->session(token));
    return m_session;
}
void* module::request::website()
{
    return m_request->website();
}
void module::request::regist(sol::state& state)
{
    // 绑定 Request 类到 Lua
    state.new_usertype<module::request>("module_request",
        "header", &module::request::header,
        "method", &module::request::method,
        "filepath", &module::request::filepath,
        "host", &module::request::host,
        "remote_ipaddress", &module::request::remote_ipaddress,
        "remote_port", &module::request::remote_port,
        "pstring", &module::request::pstring,
        "pinteger", &module::request::pinteger,
        "pnumber", &module::request::pnumber,
        "session", &module::request::session,
        "token", &module::request::token
    );
}


std::string module::request::header(const std::string& name)
{
    std::string value;
    m_request->header(name, value);
    return value;
}
ylib::network::http::method module::request::method()
{
    return m_request->method();
}
std::string module::request::filepath()
{
    return m_request->filepath();
}
std::string module::request::host()
{
    return m_request->host();
}
std::string module::request::pstring(const std::string& name)
{
    std::string value;
    bool result = request_param(name, value);
    if (result == false) {
        throw ylib::exception("request parameter '" + name + "' was not found");
    }
    return value;
}
int64 module::request::pinteger(const std::string& name)
{
    std::string value;
    bool result = request_param(name, value);
    if (result == false) {
        throw ylib::exception("request parameter '" + name + "' was not found");
    }
    return ylib::stoll(value);
}
double module::request::pnumber(const std::string& name)
{
    std::string value;
    bool result = request_param(name, value);
    if (result == false) {
        throw ylib::exception("request parameter '" + name + "' was not found");
    }
    return ylib::stod(value);
}



std::string module::request::remote_ipaddress(bool find_header, const std::string& inside_ipaddress)
{
    return m_request->remote_ipaddress(find_header, inside_ipaddress);
}
ushort module::request::remote_port()
{
    return m_request->remote_port();
}
bool module::request::request_param(const std::string& name, std::string& value)
{
    if (m_request->parser()->url_param(name, value) == false)
        return m_request->parser()->body_param(name, value);
    return true;
}
