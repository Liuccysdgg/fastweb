#pragma once
#include "base/define.h"
#include "base/error.h"
#include "base/buffer.h"
#include "util/json.h"


// HTTP
#define USE_NET_HTTP 1
// TCP
#define USE_NET_TCP 1
// UDP
#define USE_NET_UDP_NODE 1
// FTP
#define USE_NET_FTP 1






#if (USE_NET_TCP)
#define USE_NET_TCP_CLIENT 1
#define USE_NET_TCP_SERVER 1
#define USE_NET_TCP_AGENT 1
#define USE_NET_TCP_FORWARD 1
#else
#define USE_NET_TCP_CLIENT 0
#define USE_NET_TCP_SERVER 0
#define USE_NET_TCP_AGENT 0
#endif

#if (USE_NET_HTTP)
#define USE_NET_HTTP_WEBSITE 1
#define USE_NET_HTTP_CLIENT 1
#define USE_NET_HTTP_UTIL 1
#define USE_NET_HTTP_AGENT 0
#else
#define USE_NET_HTTP_WEBSITE 0
#define USE_NET_HTTP_CLIENT 0
#define USE_NET_HTTP_UTIL 0
#define USE_NET_HTTP_AGENT 0
#endif



/*请勿修改*/
#if (USE_NET_TCP_SERVER==0 || USE_NET_TCP_AGENT==0)
#ifdef USE_NET_TCP_FORWARD
#undef USE_NET_TCP_FORWARD
#endif
#define USE_NET_TCP_FORWARD 0
#else
#ifndef USE_NET_TCP_FORWARD
#undef USE_NET_TCP_FORWARD
#define USE_NET_TCP_FORWARD 1
#endif
#endif
