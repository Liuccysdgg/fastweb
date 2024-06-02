#pragma once


#include "base/singleton.hpp"
#include "core/structs.h"
#include "net/http_reqpack.h"
#include "net/http_request.h"
#include "net/http_response.h"
#include "net/http_router.h"
/// <summary>
/// 拦截器管理器
/// </summary>
class interceptor_manager{
public:
	interceptor_manager();
	~interceptor_manager();

	void load(network::http::router* router);
	void clear();
private:
	/// <summary>
	/// 服务回调
	/// </summary>
	/// <param name="reqpack"></param>
	/// <param name="express_string"></param>
	static bool callback(network::http::reqpack* reqpack, const std::string& express_string);
private:
	network::http::router* m_router = nullptr;
public:
	static std::map<std::string, std::string> interceptor;
};