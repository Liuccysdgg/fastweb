#pragma once
#include "net/define.h"
#if USE_NET_UDP_NODE
#include <functional>
#include "base/error.h"
class IUdpNode;
namespace ylib
{
	namespace network
	{
		namespace udp
		{
			class node_lst;
			class node :public ylib::error_base
			{
			public:
				node();
				~node();
				// 通过 IServer 继承
				bool start(const ylib::AddressPort& bind_ap);
				bool close();
				bool send(const std::string& remote_ipaddress, ushort remote_port, const char* pData, uint32 len);
				void on_recv(std::function<void(ylib::network::udp::node* node, const ylib::AddressPort& remote_ap, const char* data, uint32 len)> callback) { m_callback_recv = callback; }
				const ylib::AddressPort& local() { return m_local_ap; }
				friend class node_lst;
			private:
				std::function<void(ylib::network::udp::node*, const ylib::AddressPort&, const char*, uint32)> m_callback_recv;
				ylib::AddressPort m_local_ap;
				IUdpNode* m_node = nullptr;
				ylib::network::udp::node_lst* m_node_listener = nullptr;
			};
		}
	}
}
#endif