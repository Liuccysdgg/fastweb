#pragma once

#include <string>
#include <iostream>
/**
 * 基础错误类
 */

namespace ylib
{
    // 单例基类模板
	template <typename T>
	class singleton {
	public:
		// 获取单例对象的静态方法
		static T* getInstance() {
			static T instance;  // C++11以后，局部静态变量的初始化是线程安全的
			return &instance;
		}

	protected:
		// 默认构造函数
		singleton() = default;

		// 禁止拷贝构造函数和拷贝赋值操作
		singleton(const singleton&) = delete;
		singleton& operator=(const singleton&) = delete;

		// 允许析构
		virtual ~singleton() = default;
	};
}

