/************************************************************************/
/*
@author:  junliang
@brief:   跨平台的socket api封装
@time:    20141119
*/
/************************************************************************/
#ifndef _SOCKET_WRAP_H_
#define _SOCKET_WRAP_H_


#ifdef WIN32
#    include <winsock2.h>
#    include <Windows.h>
#else
#    include <unistd.h>
#    include <sys/socket.h>
#    include <sys/select.h>
#    include <sys/time.h>
#    include <netinet/in.h>
#    include <arpa/inet.h>

#    define INVALID_SOCKET -1
#endif

#include <string>
#include"mmq/inc/yy_mmq.h"

//打开网络环境
void SW_Open();

//关闭网络环境
void SW_Close();

//创建一个socket
int SW_CreateSocket();

//销毁一个socket
void SW_CloseSocket(int sd);

//创建监听socket，设置为非阻塞
int SW_Listen(const char* ip, int port);

//阻塞连接
int SW_ConnectTo(const char* ip, int port);

//非阻塞accept, 返回值判断是否accept成功
bool SW_Accept(int listen_sd, int& new_sd);

// 解析ip地址
void parse_addr(const char* addr, std::string& ip, int& port);

// 拾取一条完整消息， 消息格式是消息长度+消息内容，即YY::msg
void parse_one_msg(CONN_ID id, const char* buf, int len, int& transfer_size, CB_OnRead func);
#endif