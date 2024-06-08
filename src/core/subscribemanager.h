#pragma once


#include "core/structs.h"
#include "core/define.h"
#include "net/http_reqpack.h"
#include "net/http_request.h"
#include "net/http_response.h"
#include "net/http_router.h"
#include "util/map.hpp"
namespace fastweb
{
	/// <summary>
	/// 订阅管理器
	/// </summary>
	class subscribe_manager :public Interface {
	public:
		subscribe_manager(fastweb::app* app);
		~subscribe_manager();

		void start();
		bool add(const std::string& pattern, const std::string& filepath);
		bool remove(const std::string& pattern);
		bool exist(const std::string& pattern);
		void clear();
	private:
		/// <summary>
		/// 服务回调
		/// </summary>
		/// <param name="request"></param>
		/// <param name="response"></param>
		/// <param name="pattern"></param>
		/// <param name="filepath"></param>
		bool callback(network::http::request* request, network::http::response* response, const std::string& pattern, const std::string& filepath);
		/// <summary>
		/// 其它
		/// </summary>
		/// <param name="request"></param>
		/// <param name="response"></param>
		void other(network::http::request* request, network::http::response* response);
	private:
		/// <summary>
		/// 未找到页面
		/// </summary>
		/// <param name="response"></param>
		void send_404(network::http::response* response);
	};
}
