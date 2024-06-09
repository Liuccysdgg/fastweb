#include "timer.h"
#include "util/time.h"
#include "util/system.h"
#include "util/file.h"
#include "core/config.h"
#include "core/app.h"
#include "core/statemanager.h"
module::timer::~timer()
{
    ::ithread::stop();
    ::ithread::wait();
}
std::string module::timer::add(const std::string& name, const std::string& filepath, const std::string& funname, int msec, bool loop, sol::this_state ts)
{
    GET_APP;
    std::string filepath2;
    if (ylib::file::exist(app->config->website.dir + "/" + filepath))
        filepath2 = app->config->website.dir + "/" + filepath;
    else
    {
        return "not found script lua: " + std::string(app->config->website.dir + "/" + filepath);
    }

    std::unique_lock<std::mutex> uni(m_mutex);
    
    m_app = app;

    auto iter = m_list.find(name);
    if (iter != m_list.end())
        return "exist name `" + name + "`";

    timer_info ti;
    ti.name = name;
    ti.funname = funname;
    ti.filepath = filepath2;
    ti.loop = loop;
    ti.msec = msec;
    ti.exec_msec = time::now_msec() + msec;

    m_list.emplace(name, ti);
    m_insert = true;
    return "";
}
void module::timer::remove(const std::string& name, sol::this_state ts)
{
    m_removed.push(name);
}
void module::timer::regist(sol::state* lua)
{
    lua->new_usertype<module::timer>("timer",
        "new", sol::constructors<module::timer()>(),
        "add", &module::timer::add,
        "remove", &module::timer::remove,
        "self", &module::timer::self
    );
}

void module::timer::regist_global(const char* name, sol::state* lua)
{
    lua->registry()[name] = this;
    (*lua)[name] = this;
}
module::timer::timer()
{
    ::ithread::start();
}

bool module::timer::run()
{
    if (m_app == nullptr)
    {
        system::sleep_msec(100);
        return true;
    }
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
                ti.lua = m_app->state->get();
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
            if (m_app->config->website.debug)
                m_app->log->error("[timer][" + ti.filepath + "]: " + e.what(), __FILE__, __func__, __LINE__);
            return false;
        }
        return false;
    };
    // 获取等待时间
    auto wait_msec = comp_wait_msec();
    for (size_t i = 0; i < wait_msec/10; i++)
    {
        if (m_insert || m_state == 1)
            break;
        system::sleep_msec(10);
    }
   m_insert = false;

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
            m_app->state->push(iter->second.lua);
            m_list.erase(iter);
        }
    }
        

    return true;
}
