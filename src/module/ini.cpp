#include "ini.h"

module::ini::ini()
{
}

module::ini::~ini()
{
}

bool module::ini::open(const std::string& filepath)
{
	return m_ini.open(filepath);
}

void module::ini::close()
{
	m_ini.close();
}

std::string module::ini::read(const std::string& node, const std::string& key, const std::string& default_value) const
{
	return m_ini.read(node,key,default_value);
}

bool module::ini::write(const std::string& node, const std::string& key, const std::string& value)
{
	return m_ini.write(node,key,value);
}

bool module::ini::del(const std::string& node, const std::string& key)
{
	return m_ini.del(node, key);
}

sol::table module::ini::nodes(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    int row_num = 1;
    auto names = m_ini.names();
    for_iter(iter, names)
        result_table[row_num++] = *iter;
    return result_table;
}

sol::table module::ini::keys(const std::string& node, sol::this_state s)
{
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    int row_num = 1;
    auto names = m_ini.keys(node);
    for_iter(iter, names)
        result_table[row_num++] = *iter;
    return result_table;
}
bool module::ini::exist_key(const std::string& node, const std::string& key)
{
    return m_ini.exist_key(node, key);
}

bool module::ini::exist_node(const std::string& node)
{
    return m_ini.exist_name(node);
}

sol::table module::ini::table(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();


    auto nodes = m_ini.names();
    for_iter(iter, nodes)
    {
        sol::table node_table = lua.create_table();
        auto keys = m_ini.keys(*iter);
        for (size_t i = 0; i < keys.size(); i++)
        {
            node_table[keys[i]] = m_ini.read(*iter,keys[i]);
        }
        result_table[*iter] = node_table;
    }
    return result_table;
}

void module::ini::regist(sol::state* lua)
{
    lua->new_usertype<module::ini>("ini",
        "new", sol::constructors<module::ini()>(),
        "close", &module::ini::close,
        "del", &module::ini::del,
        "exist_key", &module::ini::exist_key,
        "exist_node", &module::ini::exist_node,
        "keys", &module::ini::keys,
        "nodes", &module::ini::nodes,
        "open", &module::ini::open,
        "read", &module::ini::read,
        "table", &module::ini::table,
        "write", &module::ini::write
    );
}
