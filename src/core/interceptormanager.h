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
	/// 拦截器管理器
	/// </summary>
	class interceptor_manager:public Interface {
	public:
		interceptor_manager(fastweb::app* app);
		~interceptor_manager();

		bool add(const std::string& pattern,const std::string& filepath);
		bool remove(const std::string& pattern);
		bool exist(const std::string& pattern);
		void clear();
	private:
		/// <summary>
		/// 服务回调
		/// </summary>
		/// <param name="reqpack"></param>
		/// <param name="pattern"></param>
		/// <param name="filepath"></param>
		bool callback(network::http::reqpack* reqpack, const std::string& pattern,const std::string& filepath);
	};
}
