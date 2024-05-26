#pragma once
#include "define.h"
#ifdef USE_NET_TCP_CLIENT
#include <string>
#include <functional>
#include "base/buffer.h"
#include "base/error.h"
class ITcpClient;
namespace ylib
{
    namespace network
    {
        namespace tcp
        {
            class client_lst;
            class client :public ylib::error_base
            {
            public:
                client(ylib::receive_model model = ylib::PUSH_DEFAULT);
                ~client();
                // 通过 IClient 继承
                bool start();
                bool start(const ylib::AddressPort& remote_ap);
                bool disconn();
                bool close(uint32 wait_msec = 0);
                bool connect(const ylib::AddressPort& remote_ap, int32 wait_msec = -1);
                bool send(const char* buff, uint32 len);
                inline const ITcpClient* getHP() { return m_client; }
                friend class client_lst;
            public:
                void on_recv(std::function<void(ylib::network::tcp::client*, const char*, uint32)> callback) { m_callback_onrecv = callback; }
                void on_accept(std::function<void(ylib::network::tcp::client*)> callback) { m_callback_onaccept = callback; }
                void on_close(std::function<void(ylib::network::tcp::client*)> callback) { m_callback_onclose = callback; }
                void on_filter(std::function<void(ylib::network::tcp::client*, uint32)> callback) { m_callback_onfilter = callback; }
            private:
                std::function<void(ylib::network::tcp::client*)> m_callback_onaccept;
                std::function<void(ylib::network::tcp::client*)> m_callback_onclose;
                std::function<void(ylib::network::tcp::client*, const char*, uint32)> m_callback_onrecv;
                std::function<void(ylib::network::tcp::client*, uint32)> m_callback_onfilter;

                ylib::AddressPort m_remote_ap;

                // 是否PULL模型
                bool m_ispull = false;

                ITcpClient* m_client = nullptr;
                client_lst* m_client_listener = nullptr;

                ylib::receive_model m_model = ylib::PUSH_DEFAULT;
            };
        }
    }
}
#endif
