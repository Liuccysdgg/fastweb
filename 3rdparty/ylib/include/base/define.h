#pragma once

#include "typedef.h"
#include "exception.h"
#include "error.h"

 
// 是否DEBUG调试内存申请情况
#define DEBUG_MEM 0

using namespace ylib;

// 斜杆
#ifdef _WIN32
#define SEPRATOR std::string("\\")
#else
#define SEPRATOR std::string("/")
#endif

// 遍历STL
#define for_iter(LOOP_VAR,LOOP_NUM) for(auto LOOP_VAR = LOOP_NUM.begin();LOOP_VAR != LOOP_NUM.end();LOOP_VAR++)
#define for_riter(LOOP_VAR,LOOP_NUM) for(auto LOOP_VAR = LOOP_NUM.rbegin();LOOP_VAR != LOOP_NUM.rend();LOOP_VAR++)
// 抛出异常
//#define THROW_FORMAT(DATA,...) throw std::exception(std::format(DATA,__VA_ARGS__).c_str())



#ifdef YLIB_STATIC
#define YLIB_API
#else

#define NEWOBJ_EXPORT_API __declspec(dllexport)
#ifdef EXPORT_DLL
#define C_DLL_HEADER extern "C" __declspec(dllexport)
#else
#define C_DLL_HEADER extern "C" __declspec(dllimport)
#endif
#define C_CALL __stdcall

#endif



#define ylib_max(a,b)            (((a) > (b)) ? (a) : (b))
#define ylib_min(a,b)            (((a) < (b)) ? (a) : (b))


/**
 * @brief 定义命名空间 ylib
 */
namespace ylib {
/**
     * @brief 定义一个矩形结构体
     *
     * 包含四个成员变量：left, top, right, bottom，用于表示矩形的位置和大小。
     * 构造函数初始化了这四个变量的值为0。
     */
    struct Rect{
        Rect() { left = 0; top = 0; right = 0; bottom = 0; } // 默认构造函数，初始化矩形的位置和大小为0
        int left;  // 矩形的左边界
        int top;   // 矩形的上边界
        int right; // 矩形的右边界
        int bottom; // 矩形的下边界
    };
    struct Geometry {
		Geometry(){
			width = 0;
			height = 0;
			x = 0;
			y = 0;
		}
        int width;
        int height;
        int x;
        int y;
    };
    /**
     * @brief 定义一个键值对结构体模板
     *
     * 模板结构体，接受两个类型参数 K 和 V。
     * 包含两个成员变量：name 和 value，分别用于存储键和值。
     */
    template<typename K,typename V>
    struct KeyValue{
        K name;  // 键
        V value; // 值
    };

    /**
     * @brief 定义一个带宽信息结构体
     */
    struct BandInfo
    {
        BandInfo() {  // 默认构造函数，初始化带宽信息的所有成员变量为0
        }
        uint64 up_all = 0;   // 上行总带宽
        uint64 down_all = 0;  // 下行总带宽
        uint64 up_sec_byte = 0; // 上行第二字节带宽
        uint64 down_sec_byte = 0; // 下行第二字节带宽
    };
    // 地址信息
    struct AddressPort {
        std::string address; // 存储地址的字符串成员
        ushort port = 0;           // 存储端口的整数成员
        //inline std::string to_string() { return address + std::to_string(port); }
    };
    // 控制台文本颜色
    enum ConsoleTextColor {
        BLUE = 0x0001,
        GREEN = 0x0002,
        RED = 0x0004,
        YELLOW = 0x0008,
        DEFAULT = RED | GREEN | BLUE,
    };
    // 文件类型
    enum FileType {
        IS_FILE,
        IS_DIRECTORY
    };
    // 磁盘容量
    struct DiskCapacity
    {
        // 用户可用剩余容量(一般用这个)
        uint64 freeBytesUser = 0;
        // 可用剩余容量
        uint64 freeBytes = 0;
        // 用户可用剩余容量
        uint64 totalBytes = 0;
    };
    // 网络接收类型
    enum receive_model
    {
        // 默认模型
        PUSH_DEFAULT,
        // 封装PACK
        PACK,
        // 自定义读取流
        PULL,
    };
    // 全路径
    class FullPath 
    {
    public:
        std::string name;
        std::string path;
    public:
        std::string to_string() { return path + SEPRATOR + name; }
    };
} // end of namespace ylib

