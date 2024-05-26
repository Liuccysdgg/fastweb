#pragma once
#include <string>
#include "base/define.h"
#include "base/buffer.h"
#include "base/error.h"
#include "util/map.hpp"
#include "net/http_define.h"
class ITcpAgent;
namespace ylib
{
	namespace network
	{
		namespace tcp
		{
			class agent_lst;
			class agent :public ylib::error_base
			{
			public:
				agent();
				~agent();
				// 启动，是否采用异步
				bool start(bool async = false, ylib::receive_model model = ylib::PUSH_DEFAULT);
				void stop();
				bool connect(const std::string& address, const ushort& port, uint64& connid, void* extra = nullptr);
				bool disConnect(uint64 connid);
				bool isConnect(uint64 connid);
				bool send(uint64 connid, const char* data, uint32 len);
				bool send(uint64 connid, const ylib::buffer& data);
				void* extra_ptr(uint64 connid);
				bool extra_ptr(uint64 connid, void* extra);
				template<typename T>
				T* extra(uint64 connid)
				{
					return (T*)extra_ptr(connid);
				}
				ITcpAgent* getHP() {
					return m_agent;
				}
				void on_handshake(std::function<void(ylib::network::tcp::agent*, uint64)> callback) { m_on_handshake_callback = callback; }
				void on_send(std::function<void(ylib::network::tcp::agent*, uint64, const char*, int)> callback) { m_on_send_callback = callback; }
				void on_receive(std::function<void(ylib::network::tcp::agent*, uint64, const char*, int)> callback) { m_on_receive_callback = callback; }
				void on_receive(std::function<void(ylib::network::tcp::agent*, uint64, int)> callback) { m_on_receive_notdata_callback = callback; }
				void on_close(std::function<void(ylib::network::tcp::agent*, uint64)> callback) { m_on_close_callback = callback; }
				void on_shutdown(std::function<void(ylib::network::tcp::agent*)> callback) { m_on_shutdown_callback = callback; }
				void on_prepareconnect(std::function<void(ylib::network::tcp::agent*, uint64)> callback) { m_on_prepareconnect_callback = callback; }
				void on_connect(std::function<void(ylib::network::tcp::agent*, uint64)> callback) { m_on_connect_callback = callback; }
				friend class agent_lst;
			public:
				std::function<void(ylib::network::tcp::agent*, uint64, const char*, int)> m_on_receive_callback;
			private:
				std::function<void(ylib::network::tcp::agent*, uint64)> m_on_handshake_callback;
				std::function<void(ylib::network::tcp::agent*, uint64, const char*, int)> m_on_send_callback;
				std::function<void(ylib::network::tcp::agent*, uint64, int)> m_on_receive_notdata_callback;
				std::function<void(ylib::network::tcp::agent*, uint64)> m_on_close_callback;
				std::function<void(ylib::network::tcp::agent*)> m_on_shutdown_callback;
				std::function<void(ylib::network::tcp::agent*, uint64)> m_on_prepareconnect_callback;
				std::function<void(ylib::network::tcp::agent*, uint64)> m_on_connect_callback;

				ITcpAgent* m_agent = nullptr;
				network::tcp::agent_lst* m_listener = nullptr;
				ylib::receive_model m_model = ylib::PUSH_DEFAULT;
			};
		}
	}
}