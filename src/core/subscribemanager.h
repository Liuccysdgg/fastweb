#pragma once


#include "sol/sol.hpp"
#include "base/singleton.hpp"
#include "core/structs.h"
#include "net/http_request.h"
#include "net/http_response.h"
#include "net/http_router.h"
/// <summary>
/// 订阅管理器
/// </summary>
class subscribe_manager{
public:
	subscribe_manager();
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
	static void other(network::http::request* request, network::http::response* response);
	/// <summary>
	/// 执行lua
	/// </summary>
	/// <param name="filepath"></param>
	/// <param name="request"></param>
	/// <param name="response"></param>
	static void exec(const std::string& filepath,network::http::request* request, network::http::response* response);
private:
	network::http::router* m_router = nullptr;
	std::vector<std::string*> m_subextra;
};