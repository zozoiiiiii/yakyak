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

//每次wsasend, wsarecv都应该提交一个新的overlap,存放本次操作
//分配释放会较频繁，用堆栈来优化
struct IOContext
{
    OVERLAPPED overlapped;            //一定要在第一位，强转用
    IO_OPERATION io_operation;        //事件
    sint64 connID;                    //连接id
    char buf[MAX_MSG_LEN*3];    //消息缓存
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
    //检查网络事件
    bool OnEventTick(float sec);

    //检查是否有新连接
    bool OnAcceptTick();

    //把指定socket绑定到完成端口上
    void UpdateCompletionPort(SOCKET socket, CONN_ID connID);

    //投递请求
    void PostRecvRequest(CONN_ID connID);
    void PostSendRequest(CONN_ID connID);

    //服务端有新连接或者客户端连接成功后，注册该socket到完成队列
    CONN_ID RegisterSocket(int sockfd);

    //创建销毁io context
    IOContext* NewIOCxt(IO_OPERATION type, CONN_ID connID);
    void DeleteIOCxt(IOContext* io_cxt);

    void ProcessWrite(CONN_ID connID, int size);
    void ProcessRead(CONN_ID connID, int size);
private:
    int m_listen_sd;
    fd_set m_read_set;        //读队列

    HANDLE m_iocp_handle;

    ////每次发送接收消息都要投递io context,所以用堆栈来缓存
    std::stack<IOContext*> m_iocxt_pool;

    //事件接口
    CB_OnCon m_CBFunc_Con;
    CB_OnDiscon m_CBFunc_Discon;
    CB_OnRead m_CBFunc_Msg;

    ConnManager m_conn_manager;            //连接管理
};

NS_YY_END