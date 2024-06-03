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
void module::file::copy_dir(const std::string& src_dir, const std::string& dst_dir)
{
    ylib::file::copy_dir(src_dir, dst_dir);
}
bool module::file::create_dir(const std::string& dirpath)
{
    return ylib::file::create_dir(dirpath,true);
}
sol::object module::file::read(const std::string& filepath, sol::this_state s)
{
    sol::object result;
    ylib::buffer data;
    if (ylib::file::read(filepath, data) == false)
        result = sol::make_object(s,data.to_string());
    else
        result = sol::make_object(s, sol::nil);
    return result;
}
bool module::file::write(const std::string& filepath, const std::string& data)
{
    return ylib::file::write(filepath, data);
}
bool module::file::remove(const std::string& filepath)
{
    return ylib::file::remove(filepath);
}
bool module::file::remove_dir(const std::string& dirpath, bool recycle)
{
    return ylib::file::remove_dir(dirpath,recycle);
}
bool module::file::exist(const std::string& filepath)
{
    return ylib::file::exist(filepath);
}
bool module::file::exist_dir(const std::string& dirpath)
{
    return ylib::file::exist_dir(dirpath);
}
int64 module::file::size(const std::string& filepath)
{
    return ylib::file::size(filepath);
}
bool module::file::copy(const std::string& src, const std::string& dst)
{
    return ylib::file::copy(src,dst);
}
timestamp module::file::last_write_time(const std::string& filepath)
{
    return ylib::file::last_write_time(filepath);
}
void module::file::regist(sol::state* lua)
{
    (*lua)["IS_FILE"] = (int)ylib::FileType::IS_FILE;
    (*lua)["IS_DIRECTORY"] = (int)ylib::FileType::IS_DIRECTORY;


    lua->new_usertype<module::file>("fs",
        "list", &module::file::list,
        "copy_dir", &module::file::copy_dir,
        "create_dir", &module::file::create_dir,
        "read",module::file::read,
        "write",module::file::write,
        "remove",module::file::remove,
        "remove_dir",module::file::remove_dir,
        "exist",module::file::exist,
        "exist_dir",module::file::exist_dir,
        "size",module::file::size,
        "copy",module::file::copy,
        "last_write_time",module::file::last_write_time
    );
}
