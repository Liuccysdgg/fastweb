#pragma once
#include "define.h"
#include "base/error.h"

#include "net/http_center.h"
#include "core/subscribemanager.h"
#include "core/interceptormanager.h"
#include "core/statemanager.h"
#include "core/config.h"
#include "core/log.h" 
#include "core/global.h"
#include "core/global_module.h"

namespace fastweb
{
	class app :public ylib::error_base {
	public:
		app();
		~app();
		bool start(const std::string& config_filepath);
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
		network::http::center* m_center = nullptr;
	public:
		// 日志
		std::shared_ptr<fastweb::log> log;
		// 订阅管理器
		std::shared_ptr<fastweb::subscribe_manager> subscribe;
		// 拦截器管理器
		std::shared_ptr<fastweb::interceptor_manager> interceptor;
		// LUA状态管理器
		std::shared_ptr<fastweb::state_manager> state;
		// 配置
		std::shared_ptr<fastweb::config> config;
		// 应用全局变量
		std::shared_ptr<fastweb::global> global;
		// 全局托管模块
		std::shared_ptr<fastweb::global_module> gloabl_module;
	};
}
