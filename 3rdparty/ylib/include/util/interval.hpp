#pragma once
#include <mutex>
#include "yutil/time.h"
namespace  ylib {
    class interval {
    public:
        interval(timestamp msec):m_interval_msec(msec),m_pre_msec(0){
            m_pre_msec = time::now_msec();
        }
        ~interval() {}
        /// <summary>
        /// 是否超市
        /// </summary>
        /// <returns></returns>
        bool isTimeout() {
            auto now_msec = time::now_msec();
            if (m_pre_msec + m_interval_msec < now_msec)
                return true;
            return false;
        }
        void timeout() {
            m_pre_msec = time::now_msec() - m_interval_msec - 1;
        }
        void update() {
            m_pre_msec = time::now_msec();
        }

    private:
        timestamp m_interval_msec = 0;
        timestamp m_pre_msec = 0;
    };
}
