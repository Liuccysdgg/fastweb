#pragma once
#include <vector>
namespace ylib
{
    template<typename T>
    struct nolock_array
    {
        nolock_array() {
            m_array = nullptr;
            m_count = 0;
        }
        ~nolock_array(){
            free();
        }
        void free(){
            if(m_array != nullptr)
                delete[] m_array;
            m_array = nullptr;
            m_count = 0;
        }
        void init(const std::vector<T>& value){
            if(value.size() == 0)
                return;
            m_array = new T[value.size()];
            m_count = value.size();
            for(size_t i=0;i<value.size();i++){
                m_array[i] = value[i];
            }
        }
        size_t append(const T& value){
            std::vector<T> v;
            v.resize(m_count+1);
            for(size_t i=0;i<m_count;i++){
                v[i] = m_array[i];
            }
            v[m_count] = value;
            free();
            init(v);
            return m_count-1;
        }
        inline T get(size_t index){
            if(index >= m_count){
                printf("Unlocked array index is too long");
                abort();
            }
            return m_array[index];
        }

        T* m_array;
        size_t m_count;
    };

}
