#pragma once
#include <map>
#include <string>
#include <mutex>
#include "base/define.h"
#include "base/log4.h"
namespace ylib
{
    class environment
    {
    public:
        template<typename T>
        T* to(const std::string& name)
        {
            std::unique_lock<std::mutex> auto_lock(m_mutex);
            return (T*)m_map[name];
        }
        template<typename T>
        void set(const std::string name,T* value)
        {
            std::unique_lock<std::mutex> auto_lock(m_mutex);
            m_map[name] = value;
        }
        void del(const std::string& name)
        {
            std::unique_lock<std::mutex> auto_lock(m_mutex);
            auto iter = m_map.find(name);
            if (iter != m_map.end())
                m_map.erase(iter);
        }
    private:
        std::mutex m_mutex;
        std::map<std::string,void*> m_map;
    };

    namespace network
    {
        namespace http
        {
            class center;
        }
    }
    namespace mysql_plus
    {
        class pool;
    }
}
namespace ylib
{
    extern ylib::environment *env;
    extern ylib::log4 *log;
}
