#pragma once
#include <tuple>
#include <memory>
#include "util/thread.h"
#include "util/sharedmem.h"
#include "net/udp_node.h"
#include "base/error.h"
/// <summary>
/// 从设备
/// </summary>
class slave:public ylib::error_base
{
public:
	slave(const std::string& name,int flag);
	~slave();
	/// <summary>
	/// 等待
	/// </summary>
	void wait();
	/// <summary>
	/// 写入
	/// </summary>
	/// <param name="data"></param>
	void write(uchar data);
private:
	std::string m_name;
	// 共享内存
	std::unique_ptr<ylib::sharedmem> m_sm;
	// 标记
	int m_flag = -1;
};