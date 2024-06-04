#include "timer.h"
#include "util/time.h"
#include "util/system.h"
#include "util/file.h"
#include "core/config.h"
#include "core/statemanager.h"
module::timer::~timer()
{
    ::ithread::stop();
    ::ithread::wait();
}
std::string module::timer::add(const std::string& name, const std::string& filepath, const std::string& funname, int msec, bool loop)
{
    std::string filepath2;
    if (ylib::file::exist(sConfig->scripts.app_dir + "/" + filepath))
        filepath2 = sConfig->scripts.app_dir + "/" + filepath;
    else
    {
        filepath2 = module_manager::getInstance()->search(filepath);
        if (filepath2.empty())
        {
            std::string result;
            result = "not found script lua, App: " + std::string(sConfig->scripts.app_dir + "/" + filepath) + "\r\n Lib: ";
            for (size_t i = 0; i < sConfig->scripts.lib_dir.size(); i++)
                result.append(std::string(sConfig->scripts.lib_dir[i] + "/" + filepath)+"\r\n");
            return result;
        }
        
    }

    std::unique_lock<std::mutex> uni(module::timer::getInstance()->m_mutex);

    auto iter = module::timer::getInstance()->m_list.find(name);
    if (iter != module::timer::getInstance()->m_list.end())
        return "exist name `" + name + "`";

    timer_info ti;
    ti.name = name;
    ti.funname = funname;
    ti.filepath = filepath2;
    ti.loop = loop;
    ti.msec = msec;
    ti.exec_msec = time::now_msec() + msec;

    module::timer::getInstance()->m_list.emplace(name, ti);
    module::timer::getInstance()->m_insert = true;
    return "";
}
void module::timer::remove(const std::string& name)
{
    module::timer::getInstance()->m_removed.push(name);
    //std::unique_lock<std::mutex> uni(module::timer::getInstance()->m_mutex);
    //module::timer::getInstance()->m_list.erase(name);
}
void module::timer::regist(sol::state* lua)
{
    lua->new_usertype<module::timer>("timer",
        "add", &module::timer::add,
        "remove", &module::timer::remove
    );
}

module::timer::timer()
{
    ::ithread::start();
}

bool module::timer::run()
{

    auto comp_wait_msec = [&]()->timestamp {
        timestamp wait_msec = 0;
        auto now_msec = time::now_msec();
        std::unique_lock<std::mutex> uni(m_mutex);
        for_iter(iter, m_list)
        {
            if (iter->second.exec_msec <= now_msec)
                return 0;
            if (iter->second.exec_msec - now_msec < wait_msec || wait_msec == 0)
                wait_msec = iter->second.exec_msec - now_msec;
        }
        if (wait_msec == 0)
            return 1000;
        return wait_msec;
    };
    auto exec_func = [&](timer_info& ti)->bool {
        std::string exception_string;
        try
        {
            // 加载LUA文件
            if (ti.lua == nullptr)
            {
                ti.lua = sStateMgr->get();
                auto lbResult = ti.lua->state->load_file(ti.filepath);
                if (lbResult.valid() == false)
                {
                    sol::error err = lbResult;
                    throw ylib::exception("Failed to load script, " + std::string(err.what()));
                }
                lbResult();
            }
            // 加载函数
            if (ti.funname != "" && ti.function.valid() == false)
                ti.function = (*ti.lua->state)[ti.funname];
            // 执行函数
            if (ti.function.valid())
            {
                auto result = ti.function();
                if (!result.valid()) {
                    sol::error err = result;
                    throw ylib::exception(err.what());
                }
                bool rd = result;
                return rd;
            }
        }
        catch (const std::exception& e)
        {
            exception_string = e.what();
            if (sConfig->website.debug)
                LOG_ERROR("[timer][" + ti.filepath + "]: " + e.what());
            return false;
        }
        return false;
    };
    // 获取等待时间
    auto wait_msec = comp_wait_msec();
    for (size_t i = 0; i < wait_msec/10; i++)
    {
        if (module::timer::getInstance()->m_insert)
            break;
        system::sleep_msec(10);
    }
    module::timer::getInstance()->m_insert = false;

    std::unique_lock<std::mutex> uni(m_mutex);
    auto now_msec = time::now_msec();
    for (auto iter = m_list.begin(); iter != m_list.end();)
    {
        if (iter->second.exec_msec <= now_msec)
        {
            bool ready_remove = false;
            if (exec_func(iter->second) == false)
                ready_remove = true;
            if (ready_remove == false)
            {
                if (iter->second.loop == false)
                    ready_remove = true;
                else
                {
                    iter->second.exec_msec = now_msec + iter->second.msec;
                }
            }
            if (ready_remove)
                iter = m_list.erase(iter);
            else
                iter++;
        }
        else
            iter++;
    }
    std::string name;
    while (m_removed.pop(name))
    {
        auto iter = m_list.find(name);
        if (iter != m_list.end())
        {
            iter->second.lua->state->collect_garbage();
            sStateMgr->push(iter->second.lua);
            m_list.erase(iter);
        }
    }
        

    return true;
}
