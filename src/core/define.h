#pragma once
#include <functional>
#include <mutex>
#include "sol/sol.hpp"
#include "base/define.h"
#include "util/system.h"
#include "util/codec.h"
#include "util/strutils.h"
#include "util/file.h"
#include "core/interface.hpp"


#define LOG_ERROR(MSG) app()->log->error(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_WARN(MSG) app()->log->warn(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_INFO(MSG) app()->log->info(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_SUCC(MSG) app()->log->success(MSG,__FILE__ ,__func__,__LINE__ )
#define LOG_DEBUG(MSG) app()->log->debug(MSG,__FILE__ ,__func__,__LINE__ )




#define GET_APP		sol::state_view lua(ts);fastweb::app* app = lua["____app"]


#define ITHREAD_WAIT_MSEC(MSEC)     \
    for(int i=0;i<(MSEC/10);i++)                \
    {                                                            \
        if (::ithread::m_state == 1)                \
            break;                                            \
        system::sleep_msec(100);                  \
    }   