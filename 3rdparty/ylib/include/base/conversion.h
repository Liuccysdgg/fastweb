#pragma once
#include <string>
#include "define.h"
#include "base/buffer.h"

namespace ylib
{
    int32 stoi(const std::string& value);
    int32 stoi(char value);
    uint64 stoull(const std::string& value);
    int64 stoll(const std::string& value);
    double stod(const std::string& value);
    float stof(const std::string& value);
    //保留小数点
    std::string to_fixed(double num, uint32 x);


    namespace bytes{
        /*流转*/
        void to_int(int32& dest, const char* src, bool reverse = false);
        void to_uint(uint32& dest, const char* src, bool reverse = false);
        void to_short(short& dest, const char* src, bool reverse = false);
        void to_ushort(ushort& dest, const char* src, bool reverse = false);
        void to_char(char* dest, short src, bool reverse = false);
        void to_char(char* dest, int32 src, bool reverse = false);
        void to_char(char* dest, int64 src, bool reverse = false);
        ylib::buffer to_buffer(short src, bool reverse = false);
        ylib::buffer to_buffer(int32 src, bool reverse = false);
        ylib::buffer to_buffer(int64 src, bool reverse = false);
        ylib::buffer to_buffer(uchar src);
        // 反转
        void reverse(void* dest, size_t length);
        void to_uint64(uint64& dest, const char* src, bool reverse = false);
        void to_int64(int64& dest, const char* src, bool reverse = false);
    }
}
