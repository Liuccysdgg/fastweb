#pragma once
#include <mutex>
namespace  ylib {
    template<typename T>
    class counter{
    public:
        counter(const T& default_value = 0){m_value = default_value;}
        ~counter() {}
        T make(){
            std::unique_lock<std::mutex> guard(m_mutex);
            return ++m_value;
        }
    private:
        T m_value;
        std::mutex m_mutex;
    };
}
