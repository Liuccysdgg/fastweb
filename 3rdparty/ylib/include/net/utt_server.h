#pragma once
#include "base/define.h"
#include "base/buffer.h"
#include "util/map.hpp"
class ITcpAgent;
class TcpAgentListener;
class UdpServerListener;
namespace ylib
{
	namespace network
	{
		class utt_server
		{
		public:
			struct Packages : public std::vector<ylib::buffer> {};
		public:
			utt_server();
			~utt_server();
			bool start(const ylib::AddressPort& bind_ap, const ylib::AddressPort& remote_ap);
			void stop();
		public:
			ylib::AddressPort m_remote_ap;
			void* m_udp_server = nullptr;
			UdpServerListener* m_udp_server_listener = nullptr;
			ITcpAgent* m_agent = nullptr;
			TcpAgentListener* m_agent_listener = nullptr;
			// 待发送
			ylib::map<uint64/*TcpAgentConnID*/, Packages*> m_wait_send_data;
		};
	}
}