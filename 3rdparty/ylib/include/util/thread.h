#pragma once
#include <functional>
#include <thread>
#include "json.h"
namespace ylib
{ 
    /****************************************************
    * Class：线程工具类
    * DateTime：2019-10-29
    ****************************************************/
    class  ithread 
    {
    public:
        ithread();
        ~ithread();
        /***************************************************************************
         * Function：开启线程
         * Return:
         *			==	true		:		成功
         *			==	false		:		失败
         ****************************************************************************/
        bool start();
        void wait();
        /***************************************************************************
         * Function：停止线程
         * Return:
         *			==	true		:		成功
         *			==	false		:		失败
         ****************************************************************************/
        bool stop();
        /// <summary>
        /// 等待关闭
        /// </summary>
        bool wait_stop();
        
        /// <summary>
        /// 睡眠等待
        /// </summary>
        void sleep(int32 msec);
    private:
        /***************************************************************************
         * Function：纯虚函数 需要子类实现
         * Description：本类会线程内调用该类,间歇时间由 Run() 内自行实现
         * Return:
         *			==	true		:		继续执行下一循环
         *			==	false		:		终止循环，退出线程
         ****************************************************************************/
        virtual bool run() = 0;
    public:
        void* __thread_handle(void* param);
    public:
        // 0=启动中 1=停止信号已发送 2=已停止
        unsigned int m_state;
        bool m_thread;
    };
}
