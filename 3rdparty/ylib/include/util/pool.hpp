#pragma once
#include <iostream>
#include <mutex>

#include <memory>
#include "base/error.h"
#include "base/exception.h"

#include "util/queue.hpp"

namespace ylib {


    class example_core{
    public:
        virtual void recover() = 0;
        virtual void task_out() = 0;
    };
    class poolcore:public ylib::error_base
    {
    public:
        void recover(void* example)
        {
            if(example == NULL)
                return;
            ((ylib::example_core*)example)->recover();

            std::unique_lock<std::mutex> sp(m_mutex);
            m_pop_size--;
            m_queue.push(example);
        }
    public:
        void* env = nullptr;
    protected:
        ylib::queue<void*> m_queue;
        std::mutex m_mutex;
        bool m_closing;
        size_t m_max_size;
        size_t m_pop_size;
    };
    enum EXAMPLE_START_RESULT
    {
        SR_TIMEOUT,
        SR_FAILED,
        SR_SUCCESS
    };
    template<typename INFO>
    class example:public example_core
    {
    public:
       
    public:
        example()
        {
            m_pool = nullptr;
        }
        virtual EXAMPLE_START_RESULT start(const INFO& info) = 0;
        virtual void close() = 0;

        inline void pool(poolcore *pool)
        {
            m_pool = pool;
        }
        inline poolcore *pool()
        {
            return m_pool;
        }
    protected:
        poolcore *m_pool;
    };

    template<typename EXAMPLE>
    //自动连接释放器
    class conn_autofree
    {
    public:
        conn_autofree(EXAMPLE* conn){
            m_conn = conn;
        }
        ~conn_autofree(){
            if (m_conn != nullptr)
            {
                m_conn->pool()->recover(m_conn);
            }
        }
        EXAMPLE* operator->() {
            if (m_conn == nullptr)
            {
                throw ylib::exception("The connection is nullptr and an exception is thrown");
            }
            return m_conn;
        }
        EXAMPLE* get(){
		if(m_conn == nullptr){
		
            throw ylib::exception("conn is nullptr");
		}
            return m_conn;
        }
    private:
        EXAMPLE* m_conn;
    };


    template<typename EXAMPLE,typename INFO>
    class pool :public poolcore
    {
    public:
        pool()
        {
            m_max_size = 0;
            m_pop_size = 0;
            m_closing = false;
        }
        ~pool()
        {
            close();
        }
        bool start(const INFO& info,size_t size)
        {
            m_closing = false;
            m_info = info;
            m_max_size = size;
            return true;
        }
        void close()
        {
            std::unique_lock<std::mutex> sp(m_mutex);
            m_closing = true;
            for (size_t i = 0; i < m_queue.size(); i++)
            {
                void* example = NULL;
                if (m_queue.pop(example) == false)
                    break;
                EXAMPLE*f = ((EXAMPLE*)example);
                ((EXAMPLE*)example)->pool(nullptr);
                delete ((EXAMPLE*)example);
            }
        }
        void* get_ptr() { return (void*)get(); }
        EXAMPLE* get()
        {
            std::unique_lock<std::mutex> sp(m_mutex);
            if (m_closing)
            {
                throw ylib::exception("pool is shutting down");
            }
            void* example = nullptr;
            if (m_queue.pop(example) == false)
            {
                if (m_pop_size < m_max_size)
                {
                    bool init_success = false;
                    example = new EXAMPLE;
                    //example->pool((void*)this);
                    ((EXAMPLE*)example)->pool(this);
                    for (uint32 i = 0; i < 3; i++)
                    {
                        auto SR = ((EXAMPLE*)example)->start(m_info);
                        if (SR == SR_SUCCESS)
                        {
                            init_success = true;    
                            break;
                        }
                        else  if (SR == SR_TIMEOUT)
                        {
                            std::cout << "start failed." << ((EXAMPLE*)example)->last_error().c_str() << std::endl;
                            std::cout << "restart " << std::to_string(i + 1).c_str() << "." << std::endl;
                        }
                        else  if (SR == SR_FAILED)
                        {
                            break;
                        }
                    }

                    if (init_success)
                    {
                        m_pop_size++;
                       
                        ((EXAMPLE*)example)->task_out();
                        return ((EXAMPLE*)example);
                    }
                    else
                    {
                        std::string last_error = ((EXAMPLE*)example)->last_error();
                        delete ((EXAMPLE*)example);
                        throw ylib::exception(last_error);
                    }
                }
                else
                {
                    throw ylib::exception("maximum capacity exceeded");
                }
            }
            else
            {
                m_pop_size++;
                ((EXAMPLE*)example)->pool(this);
                ((EXAMPLE*)example)->task_out();
                return ((EXAMPLE*)example);
            }
        }
        size_t size()
        {
            return m_max_size-m_pop_size;
        }
    public:
        INFO m_info;
    };
}
