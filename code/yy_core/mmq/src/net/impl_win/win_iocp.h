/************************************************************************/
/*
@author:  junliang
@brief:   iocp
*/
/************************************************************************/
#pragma once

#include <stack>
#include "../socket_wrap.h"
#include "../conn_manager.h"
#include"mmq/inc/yy_mmq.h"


enum IO_OPERATION
{
    ClientIoUnknown,
    ClientIoRead,
    ClientIoWrite,
};

//ÿ��wsasend, wsarecv��Ӧ���ύһ���µ�overlap,��ű��β���
//�����ͷŻ��Ƶ�����ö�ջ���Ż�
struct IOContext
{
    OVERLAPPED overlapped;            //һ��Ҫ�ڵ�һλ��ǿת��
    IO_OPERATION io_operation;        //�¼�
    sint64 connID;                    //����id
    char buf[MAX_MSG_LEN*3];    //��Ϣ����
    bool wsa_send;
};


NS_YY_BEGIN
class Win_IOCP : public YY::MMQ
{
public:
    ~Win_IOCP();
    Win_IOCP(CB_OnCon con, CB_OnDiscon discon, CB_OnRead read);
    virtual CONN_ID Connect(const char* paddr);
    virtual void Listen(const char* paddr);
    virtual void Send(CONN_ID connID, const char* buf, int len);
    virtual void Close(CONN_ID connID);
    virtual bool EventLoop(float sec);
private:
    //��������¼�
    bool OnEventTick(float sec);

    //����Ƿ���������
    bool OnAcceptTick();

    //��ָ��socket�󶨵���ɶ˿���
    void UpdateCompletionPort(SOCKET socket, CONN_ID connID);

    //Ͷ������
    void PostRecvRequest(CONN_ID connID);
    void PostSendRequest(CONN_ID connID);

    //������������ӻ��߿ͻ������ӳɹ���ע���socket����ɶ���
    CONN_ID RegisterSocket(int sockfd);

    //��������io context
    IOContext* NewIOCxt(IO_OPERATION type, CONN_ID connID);
    void DeleteIOCxt(IOContext* io_cxt);

    void ProcessWrite(CONN_ID connID, int size);
    void ProcessRead(CONN_ID connID, int size);
private:
    int m_listen_sd;
    fd_set m_read_set;        //������

    HANDLE m_iocp_handle;

    ////ÿ�η��ͽ�����Ϣ��ҪͶ��io context,�����ö�ջ������
    std::stack<IOContext*> m_iocxt_pool;

    //�¼��ӿ�
    CB_OnCon m_CBFunc_Con;
    CB_OnDiscon m_CBFunc_Discon;
    CB_OnRead m_CBFunc_Msg;

    ConnManager m_conn_manager;            //���ӹ���
};

NS_YY_END