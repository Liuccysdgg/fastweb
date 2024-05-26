#pragma once
#include "http_define.h"
#if USE_NET_HTTP_WEBSITE
#include "HPSocket/HPSocket.h"
#include <string>
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class server;
            class agent;
            class http_server_lst : public IHttpServerListener
            {
            public:

            public:
                http_server_lst(server* server);
                ~http_server_lst();
                friend class server;
            private:
                EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);
                EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);
                EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID);
                EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength);
                virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;
                EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
                EnHandleResult OnShutdown(ITcpServer* pSender);
                EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);

                EnHttpParseResult OnMessageBegin(IHttpServer* pSender, CONNID dwConnID);
                EnHttpParseResult OnRequestLine(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl);
                EnHttpParseResult OnStatusLine(IHttpServer* pSender, CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc);
                EnHttpParseResult OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue);
                EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID);
                EnHttpParseResult OnChunkHeader(IHttpServer* pSender, CONNID dwConnID, int iLength);
                EnHttpParseResult OnChunkComplete(IHttpServer* pSender, CONNID dwConnID);
                EnHttpParseResult OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType);
                EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
                EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID);
                EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc);

                EnHandleResult OnWSMessageHeader(IHttpServer* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen);
                EnHandleResult OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
                EnHandleResult OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID);
            private:
                // HPSERVER 指针
                network::http::server* m_server;
            };
        }
    }
}
#endif
