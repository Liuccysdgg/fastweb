#ifdef _WIN32
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
    std::string result;
    result = m_iter->get<std::string>(name, result);
    return result;
}

std::string module::mssql::get_datetime(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    std::tm result;
    memset(&result, 0, sizeof(result));
    result = m_iter->get<std::tm>(name, result);
    if (result.tm_year != 0)
    {
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &result);
        std::string dateTimeStr(buffer);
        return dateTimeStr;
    }
    return "";
}

int32 module::mssql::get_i32(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    int32 result = 0;
    result = m_iter->get<int32>(name, result);
    return result;
}

int64 module::mssql::get_i64(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    int64 result = 0;
    result = m_iter->get<int64>(name, result);
    return result;
}

double module::mssql::get_dob(const std::string& name)
{
    if (m_iter == m_rows.end())
        throw ylib::exception("no recorded data");
    double result = 0.0f;
    result = m_iter->get<double>(name, result);
    return result;
}
void module::mssql::query(const std::string& sql)
{
    //std::cout << "[mssql][query]: " << sql << std::endl;
    m_read = false;
    m_rows = (m_session->prepare << sql);
    //m_iter = m_rows.begin();
}
int64 module::mssql::update(const std::string& sql)
{
    //std::cout << "[mssql][update]: " << sql << std::endl;
    soci::statement st = m_session->prepare << sql;
    st.execute();
    return st.get_affected_rows();
}
bool module::mssql::next()
{
    if (m_read == false)
    {
        m_iter = m_rows.begin();

        return m_iter != m_rows.end();
    }
    if (m_iter == m_rows.end())
        return false;
    m_iter++;
    return m_iter != m_rows.end();
}


void module::mssql::regist(sol::state* lua)
{
    lua->new_usertype<module::mssql>("mssql",
        "new", sol::constructors<module::mssql(const std::string&)>(),
        "get_dob", &module::mssql::get_dob,
        "get_i32", &module::mssql::get_i32,
        "get_i64", &module::mssql::get_i64,
        "get_str", &module::mssql::get_str,
        "next", &module::mssql::next,
        "query", &module::mssql::query,
        "update", &module::mssql::update,
        "get_datetime", &module::mssql::get_datetime
    );
}
#endif