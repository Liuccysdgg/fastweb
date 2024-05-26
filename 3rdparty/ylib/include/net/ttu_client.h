#pragma once
#include <string>
#include "base/define.h"
#include "base/buffer.h"
#include "util/map.hpp"
namespace ylib::network::tcp { class server; }
class UdpClientListener;
namespace ylib
{
	namespace network
	{
		class ttu_client
		{
		public:
			struct Packages : public std::vector<ylib::buffer>
			{

			};
		public:
			ttu_client();
			~ttu_client();
			bool start(const ylib::AddressPort& listen);
			void close();
			// 设置远程地址
			void setRemoteAP(const ylib::AddressPort& remote_ap) { m_remote_ap = remote_ap; }
			void setMacConnectionCount(int count = -1) { m_max_connection_count = count; }

			size_t getConnectionCount();

			friend class UdpClientListener;
		public:
			ylib::AddressPort m_remote_ap;
			ylib::network::tcp::server* m_server = nullptr;
			// 待发送
			ylib::map<uint64, Packages*> m_wait_send_data;
			// 最大连接数
			int m_max_connection_count = -1;
		};
	}
}