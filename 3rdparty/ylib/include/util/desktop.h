#pragma once
#ifdef _WIN32
#include <vector>
#include "util/system.h"
#include "util/process.h"
#include "base/define.h"


namespace ylib
{
		/*********************************************************************************
		 * Class：桌面
		 *********************************************************************************/
        class desktop
		{
		public:
            desktop(const std::string& name,bool create_new,bool destory = true);
            desktop(size_t handle,bool destory = true);
			~desktop();
            //显示桌面
			bool show();
            //执行程序
            bool exec(const std::string& filepath, const std::string& path = "");
            //置为本桌面
			void current();
            //关闭所有创建进程
			void destoryAllProcesses(bool subproc = true);
            //取句柄
            size_t handle();
            //取桌面句柄
            size_t winhandle();
		private:
            //结束子进程
            void desktorySubProcesses(const std::list<ylib::process::proc_info>& list, size_t pid);
		private:
			// 桌面句柄
            size_t m_desktop;
            // 桌面名称
            std::string m_name;
			// 执行程序句柄
			std::vector<size_t> m_exec_hds;
			// 销毁句柄
			bool m_destory;
		public:
			/*************************************[ 公开静态函数 ]******************************************/
			struct DesktopInfo
			{
				DesktopInfo()
				{
					handle = 0;
				}
                std::string name;
                size_t handle;
			};
            //取所有桌面
			static std::vector<DesktopInfo> all(const std::string& winsta);
            //取当前桌面
            static size_t get_current();
            //显示桌面
            static void show(size_t handle);
            //名称取桌面
            static size_t get_desktop(const std::string& name);
            //关闭桌面
            static bool close(size_t desktop);
		};
}
#endif
