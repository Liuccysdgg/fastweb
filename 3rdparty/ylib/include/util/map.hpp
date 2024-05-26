#pragma once
#include <map>
#include <mutex>
#include <functional>
namespace ylib
{
	template <typename KEY, typename VAL>
	class map:private std::map<KEY,VAL>
	{
	public:
		map() {}
		~map() {}
		const std::map<KEY, VAL> to_stl()
		{
            std::unique_lock<std::mutex> __guard_lock__(m_mutex);
            return *this;
		}

		bool add(const KEY& key_, const VAL val, bool lock = true)
		{
			if (lock)
				this->m_mutex.lock();
			typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY,VAL>::find(key_);
			if (iter != ::std::map<KEY, VAL>::end()) {
				if (lock)
					this->m_mutex.unlock();
				return false;
			}

			::std::map<KEY, VAL>::insert(std::pair<KEY, VAL>(key_, val));
			if (lock)
				this->m_mutex.unlock();
			return true;
		}
		bool exist(const KEY& key_, bool lock = true)
		{



			if (lock)
				this->m_mutex.lock();
			auto iter = ::std::map<KEY,VAL>::find(key_);
			bool ret = iter != ::std::map<KEY, VAL>::end();
			if (lock)
				this->m_mutex.unlock();
			return ret;
		}
		bool set(const KEY& key_, VAL val, bool insert = false)
		{
            std::unique_lock<std::mutex> __guard_lock__(m_mutex);
			typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY, VAL>::find(key_);
			if (iter == ::std::map<KEY, VAL>::end())
			{
				if (insert == true)
				{
					::std::map<KEY, VAL>::insert(std::pair<KEY, VAL>(key_, val));
					return true;
				}
				else
				{
					return false;
				}

			}
			else
			{
				iter->second = val;
				return true;
			}
		}
		bool get(const KEY& key_, VAL& val, bool lock = true)
		{
			if (lock)
				this->m_mutex.lock();


			typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY, VAL>::find(key_);
			if (iter == ::std::map<KEY, VAL>::end()) {
				if (lock)
					this->m_mutex.unlock();
				return false;
			}

			val = iter->second;
			if (lock)
				this->m_mutex.unlock();
			return true;
		}
		bool del(const KEY& key_,bool locked = true)
        {
            if(locked)
                m_mutex.lock();
            typename std::map<KEY, VAL>::iterator iter = ::std::map<KEY, VAL>::find(key_);
            if (iter == ::std::map<KEY, VAL>::end()){
                if(locked)
                    m_mutex.unlock();
                return false;
            }

            ::std::map<KEY, VAL>::erase(iter);
            if(locked)
                m_mutex.unlock();
			return true;
		}
		void clear()
		{
            std::unique_lock<std::mutex> __guard_lock__(m_mutex);
			//::std::map<KEY,VAL>::swap();
			::std::map<KEY,VAL>::clear();
		}
		size_t size()
		{
            std::unique_lock<std::mutex> __guard_lock__(m_mutex);
			return ::std::map<KEY,VAL>::size();
		}
		void lock()
		{
			this->m_mutex.lock();
		}
		void unlock()
		{
			this->m_mutex.unlock();
		}
		VAL operator[](const KEY& key)
		{
            std::unique_lock<std::mutex> __guard_lock__(m_mutex);
			VAL val;
			get(key, val,false);
			return val;
		}
		bool find(std::function<bool(const KEY& key, const VAL& value)> delegate)
		{
            std::unique_lock<std::mutex> __guard_lock__(m_mutex);
			for_iter(iter, (*this))
			{
				if (delegate(iter->first, iter->second))
					return true;
			}
			return false;
		}
		void loop(std::function<void(const KEY& key,const VAL& value)> callback)
		{
            std::unique_lock<std::mutex> __guard_lock__(m_mutex);
			for_iter(iter, (*this))
			{
				callback(iter->first,iter->second);
			}
		}
		std::map<KEY, VAL>* parent()
		{
			return this;
		}
	public:
        std::mutex m_mutex;
	};
}

