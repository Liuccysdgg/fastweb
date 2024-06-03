#pragma once
#include "define.h"
#include "base/error.h"
#include "base/singleton.hpp"
#include "net/http_center.h"
#include "core/subscribemanager.h"
#include "core/interceptormanager.h"
class fastweb:public ylib::error_base,public ylib::singleton<fastweb> {
public:
	fastweb();
	bool start();
	void stop();
private:
	/// <summary>
	/// 初始化执行脚本
	/// </summary>
	/// <returns></returns>
	bool initialization_script();
private:
	// 初始化脚本虚拟机
	luastate* m_state_init = nullptr;
	// 网站服务核心
	network::http::center *m_center = nullptr;
	// 订阅管理器
	subscribe_manager m_subscribe;
	// 拦截器管理器
	interceptor_manager m_interceptor;
};