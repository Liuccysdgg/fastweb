#include "file.h"
#include "util/file.h"
sol::table module::file::list(const std::string& dirpath, const std::string& regex, sol::this_state s)
{
    auto map = ylib::file::traverse(dirpath, regex);

    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    int row_num = 1;

    for_iter(iter, map)
    {
        sol::table row = lua.create_table();
        row["file"] = iter->first;
        row["type"] = (int)iter->second;
        result_table[row_num++] = row;
    }
    return result_table;
}
void module::file::regist(sol::state* lua)
{
    (*lua)["IS_FILE"] = (int)ylib::FileType::IS_FILE;
    (*lua)["IS_DIRECTORY"] = (int)ylib::FileType::IS_DIRECTORY;


    lua->new_usertype<module::file>("file",
        "list", &module::file::list
    );
}
