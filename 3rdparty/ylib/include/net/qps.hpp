#pragma once
#include <mutex>
#include "base/define.h"
#include "util/system.h"

namespace ylib
{
    namespace network
    {
        class qps
        {
        public:
            qps()
            {

                m_record = false;
            }
            void request(const uint64& size, bool no_count = false)
            {
                if (m_record == false)
                    return;
                std::unique_lock<std::mutex> guard(m_mutex);
                if (no_count == false)
                    request_sec_count++;
                request_sec_size += size;

            }
            void response(const uint64& size, bool no_count = false)
            {
                if (m_record == false)
                    return;
                std::unique_lock<std::mutex> guard(m_mutex);
                if (no_count == false)
                    response_sec_count++;
                response_sec_size += size;
            }
            void record(uint32 wait_msec = 1000)
            {
                request_sec_count = 0;
                request_sec_size = 0;
                response_sec_count = 0;
                response_sec_size = 0;
                m_record = true;
                system::sleep_msec(wait_msec);
                m_record = false;
            }
        public:
            // 每秒请求数
            uint64 request_sec_count;
            // 每秒请求大小
            uint64 request_sec_size;
            // 每秒回复数
            uint64 response_sec_count;
            // 每秒回复大小
            uint64 response_sec_size;
        private:
            std::mutex m_mutex;
            // record
            bool m_record;
        };
    }
}
