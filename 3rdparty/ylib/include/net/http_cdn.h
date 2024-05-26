#pragma once
#include "http_define.h"
#include "http_reqpack.h"
#include "http_interface.h"
#include "http_client_plus.h"

#include "util/thread.h"
namespace ylib
{
    namespace network
    {
        namespace http
        {
            class cnode :public ylib::error_base
            {
            public:
                cnode(const cdn_config::node_config& config);
                ~cnode();
                const cdn_config::node_config& config() { return m_config; }
                int score();
                bool update();
            private:
                network::http::client_plus m_client;
            private:
                cdn_config::node_config m_config;
                // 管理地址
                std::string m_mang_url;
                // 回复耗时
                timestamp m_reply_wait_msec;
                // 当前带宽上传
                uint64 m_send_bytes_sec;
                // 当前带宽下载
                uint64 m_recv_bytes_sec;
                // 更新时间
                timestamp m_update_sec;
                // 通信状态
                bool m_net_status;
                // 通信状态错误描述
                std::string m_net_errormsg;
                // 最大带宽
                uint64 m_max_band;
            };

            class cdn :public network::http::interface_, public ylib::error_base, private ylib::ithread
            {
            public:
                cdn();
                ~cdn();
                bool start(const cdn_config& config);
                std::string host();
                virtual bool run() override;
                const cdn_config& config() { return m_config; }
            private:
                cdn_config m_config;
                std::vector<network::http::cnode*> m_nodes;


                std::string m_cdn_node_key;
                uint64 m_cdn_node_max_band;
                ylib::BandInfo m_cdn_node_bandinfo;
            };
        }
    }
}
