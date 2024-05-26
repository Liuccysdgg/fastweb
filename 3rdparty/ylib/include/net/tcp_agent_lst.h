#pragma once
#include "define.h"
#ifdef USE_NET_TCP_AGENT
#include <string.h>
#include "HPSocket/HPSocket.h"
namespace ylib
{
	namespace network
	{
		namespace tcp
		{
			class agent;
			class agent_lst :public ITcpAgentListener
			{
			public:
				agent_lst(ylib::network::tcp::agent* agent);
				~agent_lst();
				EnHandleResult OnHandShake(ITcpAgent* pSender, CONNID dwConnID) override;
				EnHandleResult OnSend(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
				EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
				EnHandleResult OnReceive(ITcpAgent* pSender, CONNID dwConnID, int iLength) override;
				EnHandleResult OnClose(ITcpAgent* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;
				EnHandleResult OnShutdown(ITcpAgent* pSender) override;
				EnHandleResult OnPrepareConnect(ITcpAgent* pSender, CONNID dwConnID, SOCKET socket) override;
				EnHandleResult OnConnect(ITcpAgent* pSender, CONNID dwConnID) override;
			private:
				ylib::network::tcp::agent* m_agent = nullptr;
			};
		}
	}
}
#endif
