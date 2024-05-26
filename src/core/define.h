#pragma once
#include <functional>

#include "sol/sol.hpp"

#include "base/define.h"


#include "util/system.h"
#include "util/codec.h"
#include "util/strutils.h"
#include "util/file.h"

#include "utils/logutils.h"
#include <variant>

#define LOG_ERROR(MSG) LogUtils::error(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_WARN(MSG) LogUtils::warn(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_INFO(MSG) LogUtils::info(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_SUCC(MSG) LogUtils::success(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_DEBUG(MSG) LogUtils::debug(MSG,__FILE__ ,__func__,__LINE__ )


#define sConfig 	::config::getInstance()
#define sFastWeb ::fastweb::getInstance()
#define sStateMgr ::state_manager::getInstance()



using VarType = std::variant<std::monostate,int, double, std::string,long long>;