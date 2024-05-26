#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <list>
#include "ybase/define.h"
#include "yutil/time.h"
namespace ylib
{ 


    class timeout {
    public:
        timeout() : running_(false) {}

        ~timeout() {
            stop();
        }

        void add(std::function<void(void*, const std::string&)> callback, const std::string& name, uint64_t timeout_msec, void* extra = nullptr) {
            uint64_t endTime = time::now_msec() + timeout_msec;
            TimerTask newTask = { callback, name, endTime, extra };

            {
                std::lock_guard<std::mutex> lock(mutex_);
                tasks_.push_back(newTask);
            }
        }

        void remove(const std::string& name) {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = tasks_.begin();
            while (it != tasks_.end()) {
                if (it->name == name) {
                    it = tasks_.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        void start() {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!running_) {
                running_ = true;
                schedulerThread_ = std::thread(&timeout::scheduler, this);
            }
        }

        void stop() {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                running_ = false;
            }

            if (schedulerThread_.joinable()) {
                schedulerThread_.join();
            }
        }

    private:
        struct TimerTask {
            std::function<void(void*, const std::string&)> callback;
            std::string name;
            uint64_t endTime;
            void* extra;
        };

        std::list<TimerTask> tasks_;
        std::mutex mutex_;
        std::thread schedulerThread_;
        bool running_;

        void scheduler() {
            while (running_) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                uint64_t now = time::now_msec();

                std::list<TimerTask> tasksToExecute;
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    auto it = tasks_.begin();
                    while (it != tasks_.end()) {
                        if (it->endTime <= now) {
                            tasksToExecute.push_back(*it);
                            it = tasks_.erase(it);
                        }
                        else {
                            ++it;
                        }
                    }
                }

                for (auto& task : tasksToExecute) {
                    task.callback(task.extra, task.name);
                }
            }
        }
    };

}

