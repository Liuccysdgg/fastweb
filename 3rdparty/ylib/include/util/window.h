#pragma once
#ifdef _WIN32
#include <vector>

#include "base/define.h"
 
namespace ylib{
    namespace window
    {
        //取父句柄 
        size_t parent(size_t handle);
        //取窗口类名
        std::string class_name(size_t handle);
        //取窗口标题
        std::string title_name(size_t handle);
        //置窗口标题 
        bool title_name(size_t handle, const std::string& name);
        // 查找
        size_t find(const std::string& class_name, const std::string& title_name = "");
        std::vector<size_t> find(size_t parent = 0, const std::string& class_name = "", const std::string& title_name = "", const std::string& filepath = "");
        // 取矩形  
        ylib::Rect rect(size_t handle);
        // 设置父句柄
        void set_parent(size_t handle, size_t parent);
        // 取进程路径
        std::string path(size_t handle);
        // 关闭
        void close(size_t handle);
        // 内容
        void content(size_t handle, const std::string& value);
        std::string content(size_t handle);
        namespace menu
        {
            bool click(size_t handle, size_t menu_child_handle, uint32 menu_item_idx);
            size_t get(size_t handle);
            size_t child(size_t menu_handle, uint32 menu_childs_idx);
        }
        namespace checkbox
        {
            bool checked(size_t handle);
            bool checked(size_t handle, bool checked);
        }
        namespace tab
        {
            void current(size_t handle, uint32 idx);
        }
        namespace button
        {
            bool click(size_t handle);
            bool click(size_t parent, const std::string& class_name, const std::string& title_name = "");
        }
        namespace list
        {
            void insert(size_t handle, uint32 index, const std::string& value);
        }

        void rect_sort(std::vector<size_t>& list, uint32 type, bool ps = true);
        bool enable(size_t win_handle, bool enable);
    }
    
}
#endif