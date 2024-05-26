#pragma once
#include "define.h"
#include "base/error.h"
#include "base/singleton.hpp"
#include "util/map.hpp"
class global :public ylib::error_base,public ylib::singleton<global>{
public:
	global() = default;

	void regist_lua(sol::state* lua);

	void* get_ptr(const std::string& name);
	bool regist_ptr(const std::string& name,void* value, sol::this_state ts);



private:
	ylib::map<std::string, void*> m_value_ptr;
};