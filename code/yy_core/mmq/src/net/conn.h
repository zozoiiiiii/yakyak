/************************************************************************/
/*
@author:  junliang
@brief:   一条连接
@time:    20130603
*/
/************************************************************************/
#ifndef _CONN_HPP_
#define _CONN_HPP_

#include"mmq/inc/yy_mmq.h"

//一条连接需要保存的信息
struct ConnInfo
{
    //socket
    int socket;

    //读缓存
    char r_buf[MAX_MSG_LEN];
    int r_size;

    //写缓存
    char w_buf[MAX_MSG_LEN];
    int w_size;

    //ip信息
    char remote_ip[50];
    unsigned int remote_port;
};



#endif