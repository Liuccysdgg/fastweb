#pragma once
#include <functional>
#include "net/define.h"
#include "base/error.h"
#ifdef USE_NET_TCP_SERVER
class ITcpServer;
namespace ylib
{
	namespace network
	{
		namespace tcp
		{
			class server_lst;
			/****************************************************************
			* Class: TCP服务器
			* Desc：使用HPSocket开源库。
			****************************************************************/
			class server :public ylib::error_base
			{
			public:
				server();
				~server();
				// 通过 IServer 继承
				bool start(const ylib::AddressPort& bind_ap, ylib::receive_model model = ylib::PUSH_DEFAULT);
				void close();
				bool send(uint64 connid, const char* data, uint32 len);
				bool send(uint64 connid, const ylib::buffer& data);
				bool started();
				bool disConnect(uint64 connid);
				bool isConnect(uint64 connid);
				template<typename T>
				T* extra(uint64 connid)
				{
					return (T*)extra_ptr(connid);
				}
				void* extra_ptr(uint64 connid);
				bool extra_ptr(uint64 connid, void* extra);
				inline ITcpServer* getHP() { return m_server; }
				/****************************************************************
					* Fun：[事件]连接
					****************************************************************/
				void on_accept(std::function<void(ylib::network::tcp::server* server, uint64 connid)> callback) { m_callback_accept = callback; }
				/****************************************************************
					* Fun：[事件]关闭
					****************************************************************/
				void on_close(std::function<void(ylib::network::tcp::server* server, uint64 connid)> callback) { m_callback_close = callback; }
				/****************************************************************
					* Fun：[事件]接收
					****************************************************************/
				void on_recv(std::function<void(ylib::network::tcp::server* server, uint64 connid, const char* data, uint32 len)> callback) { m_callback_recv = callback; }
				/****************************************************************
					* Fun：[事件]握手
					****************************************************************/
				void on_handshake(std::function<void(ylib::network::tcp::server* server, uint64 connid)> callback) { m_callback_handshake = callback; }
				/****************************************************************
					* Fun：[事件]已发送
					****************************************************************/
				void on_send(std::function<void(ylib::network::tcp::server* server, uint64 connid, const char* data, uint32 len)> callback) { m_callback_send = callback; }
				/****************************************************************
					* Fun：[事件]过滤接收
					****************************************************************/
				void on_recv(std::function<void(ylib::network::tcp::server* server, uint64 connid, uint32 len)> callback) { m_callback_filter = callback; }
				friend class server_lst;
			public:
				std::function<void(ylib::network::tcp::server*, uint64, const char*, uint32)> m_callback_recv;
			private:
				std::function<void(ylib::network::tcp::server*, uint64)> m_callback_accept;
				std::function<void(ylib::network::tcp::server*, uint64)> m_callback_close;
				std::function<void(ylib::network::tcp::server*, uint64, const char*, uint32)> m_callback_send;
				std::function<void(ylib::network::tcp::server*, uint64)> m_callback_handshake;
				std::function<void(ylib::network::tcp::server*, uint64, uint32)> m_callback_filter;
				ylib::receive_model m_model = ylib::PUSH_DEFAULT;

				ITcpServer* m_server = nullptr;
				ylib::network::tcp::server_lst* m_server_listener = nullptr;
			};
		}
	}
}

#endif