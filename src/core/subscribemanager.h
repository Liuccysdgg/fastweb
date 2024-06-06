#pragma once


#include "sol/sol.hpp"
#include "core/structs.h"
#include "core/define.h"
#include "net/http_request.h"
#include "net/http_response.h"
#include "net/http_router.h"
namespace fastweb
{
	/// <summary>
	/// 订阅管理器
	/// </summary>
	class subscribe_manager:public Interface {
	public:
		subscribe_manager(fastweb::app* ptr);
		~subscribe_manager();

		void load(network::http::router* router);
		void clear();
	private:
		/// <summary>
		/// 其它
		/// </summary>
		/// <param name="request"></param>
		/// <param name="response"></param>
		/// <param name="extra"></param>
		void other(network::http::request* request, network::http::response* response);
		/// <summary>
		/// 执行lua
		/// </summary>
		/// <param name="filepath"></param>
		/// <param name="request"></param>
		/// <param name="response"></param>
		void exec(const std::string& filepath, network::http::request* request, network::http::response* response);
	private:
		network::http::router* m_router = nullptr;
		std::vector<std::string*> m_subextra;
	};
}
