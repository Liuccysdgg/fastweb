#pragma once
#include <string>
#include <vector>
#include "base/define.h"
namespace ylib
{
class mem_pool
{
public:
    mem_pool(size_t initial_length = 1024);
    ~mem_pool();
    void append(const uchar* data,size_t length);
    void clear();
    size_t length() const;
    uchar* data() const;
    void resize(size_t length);
    void set(const uchar* data,size_t length);
    bool empty() const;
private:
    void renew(size_t length);
public:
    // 内存数据指针
    uchar* m_data = nullptr;
    // 内存长度
    size_t m_length = 0;
    // 使用长度
    size_t m_use_length = 0;
};
/**
     * @brief BUFF字节流
     */
class buffer 
{ 
public:
    buffer(size_t initial_length = 1024);
    buffer(char data, size_t len);
    buffer(const char* data,size_t len);
    buffer(const ylib::buffer& data);
    buffer(const std::string& data);
    buffer(std::initializer_list<uchar> char_list);
    ~buffer();
    void set(const char* data, size_t length);
    void set(const ylib::buffer& data);
    void append(const std::string& data);
    void append(const char* data, size_t length);
    void append(const ylib::buffer& data);
    void append(std::initializer_list<uchar> char_list);
    template<typename T>
    void append(const T& value) {
        append((const char*)&value, sizeof(T));
    }

    void clear();
    void resize(size_t length);
    size_t find(const char* data, size_t len, size_t start_pos = 0) const;
    size_t find(const ylib::buffer& data, size_t start_pos = 0) const;
    std::vector<size_t> find_list(const ylib::buffer& value, size_t start = 0) const;
     
    std::vector<ylib::buffer> split(const ylib::buffer& value, size_t start = 0) const;

    std::string to_string() const;
    bool empty() const;

    ylib::buffer left(size_t len) const;
    ylib::buffer right(size_t len) const;
    ylib::buffer sub(size_t start,size_t len) const;
    ylib::buffer sub(size_t start) const;
    //ylib::buffer sub(size_t start,size_t len);

    operator std::string() const;
    ylib::buffer operator+(const ylib::buffer& other) const;
    ylib::buffer& operator=(const std::string& data);
    ylib::buffer& operator=(const ylib::buffer& data);
    unsigned char& operator[](size_t index) const;

    const char* data() const;
    const size_t length() const;
    const size_t size() const { return length(); };


    ylib::buffer trim_end(char value);
    ylib::buffer trim_begin(char value);
    ylib::buffer trim(char value);

    std::string to_hex();

    size_t& cursor(){ return m_cursor; }
private:
    ylib::mem_pool m_data;
    size_t m_cursor = 0;
};
}

