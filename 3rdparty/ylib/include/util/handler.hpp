#pragma once
#include <string>
#include <vector>
#include <functional>
#include "QtCore/qobject.h"

#include "ybase/error.h"
#include "ybase/singleton.hpp"

#include "yutil/thread.h"
#include "yutil/queue.hpp"
#include "yutil/system.h"

namespace ylib
{
	template<typename DATA> class IHandler;


	template<typename DATA>
	class Handler: public ylib::error_base, public ylib::singleton<ylib::Handler<DATA>>, public ylib::ithread
	{
		friend class ylib::singleton<ylib::Handler<DATA>>;
	public:
		struct QueueInfo
		{
			int type = 0;
			int cmd = 0;
			DATA data;
			uint32 index = 0;
		};
	public: 
		void start() {
			ylib::ithread::start();
		}
		void stop() {
			ylib::ithread::stop();
			ylib::ithread::wait();
		}
		void regist(std::function<ylib::IHandler<DATA>*(int type)> callback) {
			m_callback = callback;
		};

		uint32 push(int type, int cmd,const DATA& data = DATA())
		{
			uint32 index = ++m_queue_index;
			QueueInfo info;
			info.index = index;
			info.type = (int)type;
			info.cmd = (int)cmd;
			info.data = data;
			m_queue.push(info);
			return index;
		}
		void wait(uint32 index, uint32 wait_sec = 10){
			for (size_t i = 0; i < wait_sec * 10; i++)
			{
				if (index <= m_current_queue_index)
					return;
				system::sleep_msec(100);
			}
		}
		// 获取已处理到的类型索引号
		uint32 currentIndex() {
			return m_current_queue_index;
		}
		virtual bool run() override {
			QueueInfo info;
			IHandler<DATA>* handler = nullptr;
			while (m_queue.pop(info))
			{
				handler = m_callback((int)info.type);
				handler->exec(info);
				delete handler;
				m_current_queue_index = info.index;
			}
			system::sleep_msec(100);
			return true;
		}
	private:
		Handler()
		{

		}
		~Handler()
		{

		}
	private:
		// 任务队列
		ylib::queue<QueueInfo> m_queue;
		// 已处理索引号
		uint32 m_current_queue_index = 0;
		// 索引号
		uint32 m_queue_index = 0;
		// 回调
		std::function<ylib::IHandler<DATA>*(int type)> m_callback;
	};


	template<typename DATA>
	class IHandler
	{
	public:
		virtual void exec(const struct ylib::Handler<DATA>::QueueInfo& info) = 0;
	};
}


