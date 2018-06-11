/************************************************************************/
/*
@author:    junliang
@brief:     mmq使用例子
@time:      20150706
*/
/************************************************************************/
#pragma once

#include "../inc/yy_mmq.h"
#include "base/inc/yy_varlist.h"
#include "base/inc/yy_exception.h"


struct MsgBody
{
    int id;
    char body[MAX_MSG_LEN - 8];
};

class MMQClient
{
public:
    MMQClient();
    ~MMQClient();

    void Init(int nType);

    // 客户端连接或者服务端监听
    // tcp对应的addr为 127.0.0.1:5001; shm对应的addr为 共享内存名字
    virtual void Connect(const char* paddr);

    // 关闭连接
    virtual void Close();

    // false表示没有消息。true表示有消息处理
    virtual bool EventLoop(float sec);

    // 注册感兴趣的消息
    typedef fastdelegate::FastDelegate1<const YY::VarList&> CB_OnMsg;
    virtual void RegMsg(int id, CB_OnMsg func);
    virtual void SendMsg(int msg_id, const YY::VarList& vl = YY::VarList());
private:
    void OnCon(CONN_ID id, const char* ip, int port);
    void OnDiscon(CONN_ID id, const char* ip, int port);
    void OnRead(CONN_ID id, const char* buf, int len);
private:
    std::map<int, CB_OnMsg> m_msg_dispatch;                // 消息分发
    YY::MMQ* m_mq;
    CONN_ID m_server;
};
