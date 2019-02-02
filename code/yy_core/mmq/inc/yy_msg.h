/************************************************************************/
/*
@author:    junliang
@brief:     进程间通信的消息协议
@time:      20150701
*/
/************************************************************************/
#ifndef _yy_msg_h_
#define _yy_msg_h_


#include "core/inc/yy_macros.h"
NS_YY_BEGIN


#define MAX_MSG_LEN 1024
struct Msg
{
	int len;
    char body[MAX_MSG_LEN - 4];
};
NS_YY_END
#endif