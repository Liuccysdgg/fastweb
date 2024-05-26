#pragma once
#include "define.h"
#include "util/file.h"
#include "util/thread.h"
#include "base/error.h"
#if USE_NET_FTP
namespace ylib
{
	namespace network
	{
		namespace ftp
		{
			class client;
			typedef void(*CALLBACK_FTPCLIENT_DOWNLOADING)(class ylib::network::ftp::client* client, uint32 all_size, uint32 down_size, uint64 param);
			typedef void(*CALLBACK_FTPCLIENT_DOWNLOADED)(class ylib::network::ftp::client* client, bool result, uint64 param);
			struct DownInfo
			{
				~DownInfo()
				{
					if (file != nullptr)
					{
						file->close();
						delete file;
					}
				}
				ylib::file_io* file = nullptr;
				uint32 size = 0;
				void* fileptr = nullptr;
				std::string local_path;
				std::string remote_path;
				uint64 param = 0;
				CALLBACK_FTPCLIENT_DOWNLOADING downloading = nullptr;
				CALLBACK_FTPCLIENT_DOWNLOADED downloaded = nullptr;
				bool ok = false;
			};
			class client :public ylib::error_base, private ylib::ithread
			{
			public:
				client();
				~client();
				bool connect(const std::string& ipaddress, const std::string& username, const std::string& password, ushort port);
				void close();
				bool upload(const std::string& local_filepath, const std::string& remote_filepath);
				bool download(const std::string& local_filepath, const std::string& remote_filepath, size_t param, CALLBACK_FTPCLIENT_DOWNLOADING downloading, CALLBACK_FTPCLIENT_DOWNLOADED downloaded);
				void wait();
				bool download(const std::string& local_filepath, const std::string& remote_filepath);
				bool create_dir(const std::string& remote_path);
				bool delete_dir(const std::string& remote_path);
				bool delete_file(const std::string& remote_path);
			private:
				void* m_ptr = nullptr;
				DownInfo* m_download = nullptr;
				CALLBACK_FTPCLIENT_DOWNLOADING m_cb_downloading = nullptr;
				CALLBACK_FTPCLIENT_DOWNLOADED m_cb_downloaded = nullptr;
				virtual bool run() override;
			};
		}
	}
	
};
#endif