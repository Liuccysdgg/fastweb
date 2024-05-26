#pragma once
#include "net/define.h"
#ifdef USE_NET_TCP_SERVER
#include "HPSocket/HPSocket.h"
namespace ylib
{
    namespace network
    {
        namespace tcp
        {
            class server;
            class server_lst :public CTcpServerListener
            {
            public:
                server_lst(ylib::network::tcp::server* server);
                ~server_lst();
                virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);
                virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
                virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
                virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
                virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);
                virtual EnHandleResult OnShutdown(ITcpServer* pSender);
                virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID);
                virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength);
            public:
                ylib::network::tcp::server* m_server;
            };
        }
    }
}
#endif
