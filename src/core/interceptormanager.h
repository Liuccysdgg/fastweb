#pragma once


#include "core/structs.h"
#include "core/define.h"
#include "net/http_reqpack.h"
#include "net/http_request.h"
#include "net/http_response.h"
#include "net/http_router.h"
namespace fastweb
{
	/// <summary>
	/// 拦截器管理器
	/// </summary>
	class interceptor_manager:public Interface {
	public:
		interceptor_manager(fastweb::app* app);
		~interceptor_manager();

		void load(network::http::router* router);
		void clear();

	private:
		/// <summary>
		/// 服务回调
		/// </summary>
		/// <param name="reqpack"></param>
		/// <param name="express_string"></param>
		bool callback(network::http::reqpack* reqpack, const std::string& express_string);
	private:
		network::http::router* m_router = nullptr;
		std::map<std::string, std::string> interceptor;
	};
}
