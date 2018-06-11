/************************************************************************/
/*
@author:  junliang
@brief:   һ������
@time:    20130603
*/
/************************************************************************/
#ifndef _CONN_HPP_
#define _CONN_HPP_

#include"mmq/inc/yy_mmq.h"

//һ��������Ҫ�������Ϣ
struct ConnInfo
{
    //socket
    int socket;

    //������
    char r_buf[MAX_MSG_LEN];
    int r_size;

    //д����
    char w_buf[MAX_MSG_LEN];
    int w_size;

    //ip��Ϣ
    char remote_ip[50];
    unsigned int remote_port;
};



#endif