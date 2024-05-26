#pragma once
#include <queue>
#include <stdio.h>
#include <mutex>
namespace ylib
{
    template<typename T>
    class queue
    {
    public:
        struct node
        {
            T value;
            node* next;
        };
    public:
        queue()
        {
            m_first = nullptr;
            m_end = nullptr;
            m_size = 0;
        }
        ~queue()
        {
            clear();
        }
        inline void clear()
        {
            m_mutex.lock();
            T value;
            while(pop(value,false));
            m_mutex.unlock();

        }
        inline void push(const T& value, bool locked = true)
        {
            if(locked){
                m_mutex.lock();
            }
            m_size++;
            if(m_first ==nullptr)
            {
                m_first = new node();
                m_first->next  =  nullptr;
                m_first->value = value;
                m_end = m_first;
                if(locked){
                    m_mutex.unlock();
                }
                return;
            }
            m_end->next = new node;
            m_end->next->next = nullptr;
            m_end->next->value = value;
            m_end = m_end->next;
            if(locked){
                m_mutex.unlock();
            }
        }
        inline bool pop(T& value,bool locked = true)
        {
            if(m_first == nullptr)
                return false;

            if(locked){
                m_mutex.lock();
            }

            if(m_first == nullptr)
            {
                if(locked){
                    m_mutex.unlock();
                }
                return false;
            }

            m_size--;
            value = m_first->value;
            node* temp = m_first->next;
            delete m_first;
            m_first = temp;
            if(locked){
                m_mutex.unlock();
            }
            return true;
        }
        inline size_t size(){return m_size;}
        node* m_first;
        node* m_end;
        std::mutex m_mutex;
        size_t m_size;
    };
}

