/************************************************************************/
/*
@author:  junliang
@brief:   ��ƽ̨��socket api��װ
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

//�����绷��
void SW_Open();

//�ر����绷��
void SW_Close();

//����һ��socket
int SW_CreateSocket();

//����һ��socket
void SW_CloseSocket(int sd);

//��������socket������Ϊ������
int SW_Listen(const char* ip, int port);

//��������
int SW_ConnectTo(const char* ip, int port);

//������accept, ����ֵ�ж��Ƿ�accept�ɹ�
bool SW_Accept(int listen_sd, int& new_sd);

// ����ip��ַ
void parse_addr(const char* addr, std::string& ip, int& port);

// ʰȡһ��������Ϣ�� ��Ϣ��ʽ����Ϣ����+��Ϣ���ݣ���YY::msg
void parse_one_msg(CONN_ID id, const char* buf, int len, int& transfer_size, CB_OnRead func);
#endif