/************************************************************************/
/*
@author:    junliang
@brief:     ���̼�ͨ�ŵ���ϢЭ��
@time:      20150701
*/
/************************************************************************/
#ifndef _yy_msg_h_
#define _yy_msg_h_


#include "core/inc/yy_macros.h"
NS_YY_BEGIN


// ��Ϣ��С����
#define MAX_MSG_LEN 1024*50
struct Msg
{
    int len;
    char body[MAX_MSG_LEN - 4];
};
NS_YY_END
#endif