#include "filesystem.h"
#include "util/file.h"
sol::table module::filesystem::list(const std::string& dirpath, const std::string& regex, sol::this_state s)
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
void module::filesystem::copy_dir(const std::string& src_dir, const std::string& dst_dir)
{
    ylib::file::copy_dir(src_dir, dst_dir);
}
bool module::filesystem::create_dir(const std::string& dirpath)
{
    return ylib::file::create_dir(dirpath, true);
}
sol::object module::filesystem::read(const std::string& filepath, sol::this_state s)
{
    sol::object result;
    ylib::buffer data;
    if (ylib::file::read(filepath, data) == false)
        result = sol::make_object(s, data.to_string());
    else
        result = sol::make_object(s, sol::nil);
    return result;
}
bool module::filesystem::write(const std::string& filepath, const std::string& data)
{
    return ylib::file::write(filepath, data);
}
bool module::filesystem::remove(const std::string& filepath)
{
    return ylib::file::remove(filepath);
}
bool module::filesystem::remove_dir(const std::string& dirpath, bool recycle)
{
#ifdef _WIN32
    return ylib::file::remove_dir(dirpath, recycle);
#else
    return ylib::file::remove_dir(dirpath);
#endif
}
bool module::filesystem::exist(const std::string& filepath)
{
    return ylib::file::exist(filepath);
}
bool module::filesystem::exist_dir(const std::string& dirpath)
{
    return ylib::file::exist_dir(dirpath);
}
int64 module::filesystem::size(const std::string& filepath)
{
    return ylib::file::size(filepath);
}
bool module::filesystem::copy(const std::string& src, const std::string& dst)
{
    return ylib::file::copy(src, dst);
}
timestamp module::filesystem::last_write_time(const std::string& filepath)
{
    return ylib::file::last_write_time(filepath);
}
void module::filesystem::regist(sol::state* lua)
{
    (*lua)["IS_FILE"] = (int)ylib::FileType::IS_FILE;
    (*lua)["IS_DIRECTORY"] = (int)ylib::FileType::IS_DIRECTORY;


    lua->new_usertype<module::filesystem>("fs",
        "list", &module::filesystem::list,
        "copy_dir", &module::filesystem::copy_dir,
        "create_dir", &module::filesystem::create_dir,
        "read", module::filesystem::read,
        "write", module::filesystem::write,
        "remove", module::filesystem::remove,
        "remove_dir", module::filesystem::remove_dir,
        "exist", module::filesystem::exist,
        "exist_dir", module::filesystem::exist_dir,
        "size", module::filesystem::size,
        "copy", module::filesystem::copy,
        "last_write_time", module::filesystem::last_write_time
    );
}
