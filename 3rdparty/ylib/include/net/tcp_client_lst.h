#pragma once
#include "define.h"
#ifdef USE_NET_TCP_CLIENT
#include <string.h>
#include "HPSocket/HPSocket.h"
namespace ylib
{
    namespace network
    {
        namespace tcp
        {
            /*接收数据的线程池调用线程，用于再调用用户提供的指针，主要是考虑到 m_sportMode 模式才会使用到该函数*/
            void __HP_CALL callback_thread_recv_tcpclient(PVOID param);
            class client;
            class client_lst :public CTcpClientListener
            {
            public:
                client_lst(client* client);
                ~client_lst();
                friend class client;
            private:
                virtual EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override;
                virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override;
                virtual EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override;
                virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
                virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
                virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;
                virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) override;
            private:
                int m_state;
                ylib::network::tcp::client* m_client;
            };
        }
    }
}
#endif
