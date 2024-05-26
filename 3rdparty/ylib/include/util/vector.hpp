#pragma once
#include <vector>
#include <functional>
namespace ylib
{
template<typename T>
class vector:public std::vector<T>
{
public:
    vector(const std::vector<T>& value):std::vector<T>()
    {
        operator=(value);
    }
    vector() : std::vector<T>()
    {

    }
    ~vector()
    {

    }
    void operator=(const std::vector<T>& value)
    {
        *((std::vector<T>*)this) = value;
    }
    bool find(std::function<bool(T& value, size_t idx)> callback)
    {
        for (size_t i = 0; i < this->size(); i++)
        {
            if (callback((*this)[i], i))
                return true;
        }
        return false;
    }
    bool find(const T& value)
    {
        for (size_t i = 0; i < this->size(); i++)
        {
            if(value == this->at(i))
                return true;
        }
        return false;
    }
    bool find(std::function<bool(T& value, size_t idx)> callback,T& value)
    {
        for (size_t i = 0; i < this->size(); i++)
        {
            if (callback((*this)[i], i))
            {
                value =this->at(i);
                return true;
            }
        }
        return false;
    }
    void loop(std::function<void(const T& value)> callback)
    {
        for (size_t i = 0; i < this->size(); i++)
            callback(this->at(i));
    }
    void loop_fl(std::function<void(const T& value,bool first,bool last)> callback)
    {
        for (size_t i = 0; i < this->size(); i++)
            callback(this->at(i),i==0,i==this->size()-1);
    }
    void rloop(std::function<void(const T& value)> callback)
    {
        size_t size = this->size();
        for (size_t i = 0; i < this->size(); i++)
            callback(this->at(size-i-1));
    }
    void rloop_fl(std::function<void(const T& value, bool first, bool last)> callback)
    {
        size_t size = this->size();
        for (size_t i = 0; i < this->size(); i++)
            callback(this->at(size - i - 1), i == size - 1, i == 0);
    }
};
}
