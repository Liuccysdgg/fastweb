#pragma once
#include "net/tcp2tcp.h"
#include "util/map.hpp"
namespace ylib
{
	namespace network
	{
		namespace tcp
		{
			class forward
			{
			public:
				struct Packages : public std::vector<ylib::buffer>
				{

				};
			public:
				forward();
				~forward();
				bool start(const std::string& listen_address, ushort listen_port);
				void stop();
				void setAP(const ylib::AddressPort& remote) { m_remote = remote; }
				void setMaxConntionCount(uint32 max) { m_max_connection_count = max; }
				uint64 connectionCount();
				void disIpAddress(const std::string& ipaddress);
			private:
				ylib::AddressPort m_remote;
				network::tcp::tcp2tcp m_t2t;
				uint32 m_max_connection_count = 0;
				// 待发送
				ylib::map<uint64, Packages*> m_wait_send_data;
			};
		}
	}
}