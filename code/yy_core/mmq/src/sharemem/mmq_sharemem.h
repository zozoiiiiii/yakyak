/************************************************************************/
/*
@author:    junliang
@brief:     共享内存， 为了和网络库统一，引进CONN_ID, 内部用不到。
@time:      20150701
*/
/************************************************************************/
#ifndef _mmq_share_mem_h_
#define _mmq_share_mem_h_

#include <memory>
#include"mmq/inc/yy_mmq.h"
#include "util/inc/yy_sharemem.h"
#include "core/inc/yy_varlist.h"

//共享内存中的消息队列
#define MAX_SB_MSG_QUEUE_LEN 1024

NS_YY_BEGIN
struct MsgQueue
{
    int nreadindex;        // 读进程使用
    int nwriteindex;    // 写进程使用
    YY::Msg msg_queue[MAX_SB_MSG_QUEUE_LEN];            // 数据

    MsgQueue()
    {
        nreadindex = 0;
        nwriteindex = 0;
        memset(msg_queue, 0, sizeof(YY::Msg)*MAX_SB_MSG_QUEUE_LEN);
    }
};

struct sharememoryinfo
{
    MsgQueue svrread;            // 服务器进程写队列
    MsgQueue svrwrite;            // 服务器进程读队列
};

class MMQ_ShareMem : public MMQ
{
public:
    MMQ_ShareMem(CB_OnCon con, CB_OnDiscon discon, CB_OnRead read);
    virtual ~MMQ_ShareMem();
    virtual CONN_ID Connect(const char* paddr);
    virtual void Listen(const char* paddr);
    virtual void Close(CONN_ID);
    virtual void Send(CONN_ID, const char* pbuf, int len);
    virtual bool EventLoop(float sec);

private:
    // 往队列中写消息
    static void Append(MsgQueue* pmsgqueue, const YY::Msg* pmsg);

    // 队列是否为空
    static bool IsEmpty(MsgQueue* pmsgqueue);

    // 从队列中读消息
    static void Pop(MsgQueue* pmsgqueue, YY::Msg* pmsg);
private:
    CB_OnRead m_CBFunc_Msg;                    // 有消息可读时的回调
    ShareMem<sharememoryinfo> m_sharemem;        // 共享内存管理
};

NS_YY_END
#endif