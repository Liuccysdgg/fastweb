#include "mssql.h"
#include "soci/odbc/soci-odbc.h"
module::mssql::mssql(const std::string& connstring)
{
    m_session = std::make_shared<soci::session>(soci::odbc, connstring);
}

module::mssql::~mssql()
{
    
}
std::string module::mssql::get_str(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    return m_iter->get<std::string>(name);
}

int32 module::mssql::get_i32(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    return m_iter->get<int32>(name);
}

int64 module::mssql::get_i64(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    return m_iter->get<int64>(name);
}

double module::mssql::get_dob(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    return m_iter->get<double>(name);
}
void module::mssql::query(const std::string& sql)
{
    m_read = false;
    m_rows = (m_session->prepare << sql);
    //m_iter = m_rows.begin();
}
uint64 module::mssql::update(const std::string& sql)
{
    soci::statement st = (m_session->prepare << sql);
    return st.get_affected_rows();
}
bool module::mssql::next()
{
    if (m_read == false)
    {
        m_iter = m_rows.begin();
        return true;
    }
    if (m_iter == m_rows.end())
        return false;
    m_iter++;
    return true;
}


void module::mssql::regist(sol::state& lua)
{
    lua.new_usertype<module::mssql>("mssql",
        "new", sol::constructors<module::mssql(const std::string&)>(),
        "get_dob", &module::mssql::get_dob,
        "get_i32", &module::mssql::get_i32,
        "get_i64", &module::mssql::get_i64,
        "get_str", &module::mssql::get_str,
        "next", &module::mssql::next,
        "query", &module::mssql::query,
        "update", &module::mssql::update
    );
}
