#pragma once
#include "net/define.h"
#if USE_NET_UDP_NODE
#include "HPSocket/HPSocket.h"
#include <string>
namespace ylib
{
    namespace network
    {
        namespace udp
        {
            class node;
            class node_lst :public CUdpNodeListener
            {
            public:
                node_lst(ylib::network::udp::node* node);
                ~node_lst();
            private:
                virtual EnHandleResult OnPrepareListen(IUdpNode* pSender, SOCKET soListen);
                virtual EnHandleResult OnSend(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
                virtual EnHandleResult OnReceive(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
                virtual EnHandleResult OnError(IUdpNode* pSender, EnSocketOperation enOperation, int iErrorCode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength);
                virtual EnHandleResult OnShutdown(IUdpNode* pSender);
            private:
                ylib::network::udp::node* m_node = nullptr;
            };
        }
    }
}
#endif