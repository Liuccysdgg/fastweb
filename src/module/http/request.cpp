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
module::session* module::request::session(const std::string& token)
{
    if (m_session == nullptr)
        m_session = new module::session(&m_request->session(token));
    return m_session;
}
sol::table module::request::body_param(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    auto &map = m_request->body_param();
    for_iter(iter,(*map.get()))
        result_table[iter->first] = iter->second;
    return result_table;
}
sol::table module::request::url_param(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    auto& map = m_request->url_param();
    for_iter(iter, (*map.get()))
        result_table[iter->first] = iter->second;
    return result_table;
}
std::string module::request::body()
{
    return m_request->body();
}
sol::table module::request::multipart(sol::this_state s)
{
    auto multipart = m_request->multipart();
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();

    int count = 1;
    for (size_t i = 0; i < multipart->size(); i++)
    {
        sol::table file = lua.create_table();
        {
            sol::table param = lua.create_table();
            for_iter(iter, multipart->at(i).param)
            {
                param[iter->first] = iter->second;// multipart->at(i).header;
                file["param"] = param;
            }
        }
        file["id"] = i+1;
        file["size"] = multipart->at(i).length;
        result_table[count] = file;
        count++;
    }
    return result_table;
}
std::string module::request::multipart_content(int id)
{
    multipart_content_check(id);
    auto mult_info = m_request->multipart()->at(id - 1);
    return std::string(m_request->body().data() + mult_info.offset, mult_info.length);
}
bool module::request::multipart_content_save(int id, const std::string& filepath)
{
    ylib::file_io file;
    if (file.open(filepath) == false)
    {
        throw ylib::exception("open file failed," + file.last_error());
    }
    multipart_content_check(id);
    auto mult_info = m_request->multipart()->at(id - 1);
    if (file.write(m_request->body().data() + mult_info.offset, mult_info.length) == false)
    {
        throw ylib::exception("write file failed,"+file.last_error());
        return false;
    }
    file.close();
    return true;
}
//void module::request::set(const std::string& name, const sol::object& obj, sol::this_state ts)
//{
//    /*auto type = obj.get_type();
//    if (obj.get_type() == sol::type::string)*/
//        m_request->reqpack()->extra()[name] = obj.as<std::string>();
//    //else if (obj.get_type() == sol::type::number)
//    //    m_request->reqpack()->extra()[name] = obj.as<double>();
//    //else if (obj.get_type() == sol::type::boolean)
//    //    m_request->reqpack()->extra()[name] = obj.as<bool>();
//    //else
//        //throw ylib::exception("Unsupported temporary data");
//}
//sol::object module::request::get(const std::string& name, sol::this_state ts)
//{
//    //if (m_request->reqpack()->extra()[name].is_string())
//        return sol::make_object(ts, m_request->reqpack()->extra()[name].to<std::string>());
//    //else if (m_request->reqpack()->extra()[name].is_number())
//    //    return sol::make_object(ts, m_request->reqpack()->extra()[name].to<double>());
//    //else if (m_request->reqpack()->extra()[name].is_bool())
//    //    return sol::make_object(ts, m_request->reqpack()->extra()[name].to<bool>());
//    //else
////        return sol::make_object(ts,sol::nil);
//}
void module::request::set(const std::string& name, const std::string& value)
{
    m_request->reqpack()->extra()[name] = value;
} 
std::string module::request::get(const std::string& name)
{
    return m_request->reqpack()->extra()[name].to<std::string>();
}
network::http::website* module::request::website()
{
    return m_request->website();
}
void module::request::regist(sol::state* lua)
{
    // 绑定 Request 类到 Lua
    lua->new_usertype<module::request>("fw_request",
        "header", &module::request::header,
        "method", &module::request::method,
        "filepath", &module::request::filepath,
        "host", &module::request::host,
        "remote_ipaddress", &module::request::remote_ipaddress,
        "remote_port", &module::request::remote_port,
        "param", &module::request::param,
        "session", &module::request::session,
        "body_param", &module::request::body_param,
        "url_param", &module::request::url_param,
        "body", &module::request::body,
        "multipart", &module::request::multipart,
        "multipart_content", &module::request::multipart_content,
        "multipart_content_save", &module::request::multipart_content_save,
        "get", &module::request::get,
        "set", &module::request::set
    );
}

void module::request::multipart_content_check(int id)
{
    if (m_request->multipart()->size() < id)
    {
        throw ylib::exception("The maximum ID is " + std::to_string(m_request->multipart()->size()) + ", and the incoming ID is " + std::to_string(id));
    }
    auto mult_info = m_request->multipart()->at(id - 1);
    if (mult_info.offset + mult_info.length > m_request->body().length())
    {
        throw ylib::exception("Fastweb internal exception, offset: " + std::to_string(mult_info.offset) + " length: " + std::to_string(mult_info.length) + " body: " + std::to_string(m_request->body().length()));
    }
}
std::string module::request::header(const std::string& name)
{
    std::string value;
    m_request->header(name, value);
    return value;
}
std::string module::request::method()
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
sol::object module::request::param(const std::string& name, bool throw_,sol::this_state s)
{
    std::string value;
    bool result = request_param(name, value);
    if (result == false){
        if (throw_)
            throw ylib::exception("request parameter '" + name + "' was not found");
        else
            return sol::make_object(s, sol::nil);
    }
    return sol::make_object(s,value);
}

std::string module::request::remote_ipaddress()
{
    return m_request->remote().address;
}
ushort module::request::remote_port()
{
    return m_request->remote().port;
}
bool module::request::request_param(const std::string& name, std::string& value)
{
    if (m_request->get_url_param(name, value) == false)
        return m_request->get_body_param(name, value);
    return true;
}
