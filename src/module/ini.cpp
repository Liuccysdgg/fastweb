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
