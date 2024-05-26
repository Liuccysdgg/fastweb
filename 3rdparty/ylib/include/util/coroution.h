#pragma once
#if 0
#include <coroutine>
#include <iostream>
#include <thread>
#include <chrono>
#include "util/thread.h"
#include "util/queue.hpp"
class co_thread_pool;
namespace ylib::co
{
    class coroutine {
        public:
            struct promise_type {
                coroutine get_return_object() {
                    return coroutine{ std::coroutine_handle<promise_type>::from_promise(*this) };
                }
                std::suspend_always initial_suspend() { return {}; }  // 协程初始化时挂起
                std::suspend_never final_suspend() noexcept { return {}; } // 协程结束时挂起
                void return_void() {}
                void unhandled_exception() { std::exit(1); }
            };
            coroutine(std::coroutine_handle<promise_type> h) : coro(h) {}
            ~coroutine() { if (coro) coro.destroy(); }
            std::coroutine_handle<promise_type> coro;
    };

    /// <summary>
    /// 协程调度器
    /// </summary>
    class scheduler :public ylib::ithread {
    public:
        // 任务信息
        struct task_info {
            // 任务回调
            std::function<void(void*,ylib::co::scheduler*)> callback;
            // 任务参数
            void* param = nullptr;
            // 唤醒协程coco
            std::coroutine_handle<>* coco = nullptr;
        };
    public:
        scheduler();
        ~scheduler();
        /// <summary>
        /// 启动
        /// </summary>
        /// <param name="thread_size">挂起协程执行任务线程池大小</param>
        /// <returns></returns>
        bool start(uint32 thread_size);
        void stop();
        /// <summary>
        /// 投递协程
        /// </summary>
        /// <param name="info"></param>
        void push(const task_info& info);
        /// <summary>
        /// 投递线程任务
        /// </summary>
        /// <param name="callback"></param>
        void push_t(std::function<void()> callback);
        /// <summary>
        /// 唤醒协程
        /// </summary>
        /// <param name="continuation"></param>
        void resume(std::coroutine_handle<>* continuation);
    private:
        // 通过 ithread 继承
        bool run() override;
        /// <summary>
        /// 处理队列
        /// </summary>
        void exec_queue();
    private:
        // 协程处理队列
        ylib::queue<task_info> m_queue;
        // 线程池
        co_thread_pool* m_pool = nullptr;
    };
}
#endif