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
#include "mmq_client.h"


class MMQSvr
{
public:
    MMQSvr();
    ~MMQSvr();
    void Init(int nType);

    // 客户端连接或者服务端监听
    // tcp对应的addr为 127.0.0.1:5001; shm对应的addr为 共享内存名字
    virtual void Listen(const char* paddr){m_mq->Listen(paddr);}

    // 关闭连接
    virtual void Close(CONN_ID id){m_mq->Close(id);}

    // false表示没有消息。true表示有消息处理
    virtual bool EventLoop(float sec){return m_mq->EventLoop(sec);}

    // 注册感兴趣的消息
    typedef fastdelegate::FastDelegate2<CONN_ID, const YY::VarList&> CB_OnMsg;
    virtual void RegMsg(int id, CB_OnMsg func);
    virtual void SendMsg(CONN_ID id, int msg_id, const YY::VarList& vl = YY::VarList());
private:
    void OnCon(CONN_ID id, const char* ip, int port);
    void OnDiscon(CONN_ID id, const char* ip, int port);
    void OnRead(CONN_ID id, const char* buf, int len);
private:
    std::map<int, CB_OnMsg> m_msg_dispatch;                // 消息分发
    YY::MMQ* m_mq;
};
