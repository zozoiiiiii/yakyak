/************************************************************************/
/*
@author:    junliang
@brief:     �����ڴ棬 Ϊ�˺������ͳһ������CONN_ID, �ڲ��ò�����
@time:      20150701
*/
/************************************************************************/
#ifndef _mmq_share_mem_h_
#define _mmq_share_mem_h_

#include <memory>
#include"mmq/inc/yy_mmq.h"
#include "util/inc/yy_sharemem.h"
#include "core/inc/yy_varlist.h"

//�����ڴ��е���Ϣ����
#define MAX_SB_MSG_QUEUE_LEN 1024

NS_YY_BEGIN
struct MsgQueue
{
    int nreadindex;        // ������ʹ��
    int nwriteindex;    // д����ʹ��
    YY::Msg msg_queue[MAX_SB_MSG_QUEUE_LEN];            // ����

    MsgQueue()
    {
        nreadindex = 0;
        nwriteindex = 0;
        memset(msg_queue, 0, sizeof(YY::Msg)*MAX_SB_MSG_QUEUE_LEN);
    }
};

struct sharememoryinfo
{
    MsgQueue svrread;            // ����������д����
    MsgQueue svrwrite;            // ���������̶�����
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
    // ��������д��Ϣ
    static void Append(MsgQueue* pmsgqueue, const YY::Msg* pmsg);

    // �����Ƿ�Ϊ��
    static bool IsEmpty(MsgQueue* pmsgqueue);

    // �Ӷ����ж���Ϣ
    static void Pop(MsgQueue* pmsgqueue, YY::Msg* pmsg);
private:
    CB_OnRead m_CBFunc_Msg;                    // ����Ϣ�ɶ�ʱ�Ļص�
    ShareMem<sharememoryinfo> m_sharemem;        // �����ڴ����
};

NS_YY_END
#endif