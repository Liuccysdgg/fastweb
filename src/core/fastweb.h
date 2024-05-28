#pragma once
#include "define.h"
#include "base/error.h"
#include "base/singleton.hpp"
#include "net/http_center.h"
#include "net/http_response.h"
#include "net/http_request.h"
#include "core/bytecodemanager.h"
class fastweb:public ylib::error_base,public ylib::singleton<fastweb> {
public:
	fastweb() = default;
	bool start();
	void stop();

	/// <summary>
	/// 发送文件
	/// </summary>
	/// <param name="response"></param>
	/// <param name="filepath"></param>
	void send_file(network::http::response* response, std::string filepath);
	/// <summary>
	/// 发送404
	/// </summary>
	/// <param name="response"></param>
	void send_404(network::http::response* response);
private:
	/// <summary>
	/// 初始化执行脚本
	/// </summary>
	/// <returns></returns>
	bool initialization_script();
	/// <summary>
	/// 服务回调
	/// </summary>
	/// <param name="request"></param>
	/// <param name="response"></param>
	static void subscribe_service(network::http::request* request, network::http::response* response, void* extra);
	/// <summary>
	/// 拦截器回调
	/// </summary>
	static bool subscribe_interceptor(network::http::reqpack* reqpack,const std::string& express_string);
private:
	network::http::center *m_center = nullptr;
public:
	// 服务字节码
	bytecode_manager service_bytecode;
	// 拦截器字节码
	bytecode_manager interceptor_bytecode;
};