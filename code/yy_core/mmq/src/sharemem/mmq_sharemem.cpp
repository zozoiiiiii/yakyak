#include "mmq_sharemem.h"
#include "core/inc/yy_exception.h"
NS_YY_BEGIN

MMQ_ShareMem::~MMQ_ShareMem(){}


MMQ_ShareMem::MMQ_ShareMem(CB_OnCon con, CB_OnDiscon discon, CB_OnRead read)
{
    m_CBFunc_Msg = read;
}

void MMQ_ShareMem::Close(CONN_ID)
{
    if (m_nRoleType == RT_UNKNOWN)
    {
        return;
    }

    m_sharemem.close();

    // 重新初始化。防止eventloop中崩溃
    m_nRoleType = RT_UNKNOWN;
}

CONN_ID MMQ_ShareMem::Connect(const char* pdata)
{
    // 共享内存管理
    m_sharemem.open(pdata);
    m_nRoleType = RT_CONNECT;
    return INVALID_CONN_ID;
}

void MMQ_ShareMem::Listen(const char* pdata)
{
    // 打开共享内存
    m_sharemem.open(pdata);

    // 如果是listen, 则清理数据; 会导致connect后立马发送的消息删除掉，所以此处不必清理，只要第一个创建者清理足矣
    //m_sharemem.clear();

    m_nRoleType = RT_LISTEN;
}



void MMQ_ShareMem::Send(CONN_ID, const char* pbuf, int len)
{
    throw_assert(m_nRoleType != RT_UNKNOWN, "role type is unknown.");

    // 确保消息长度合法
    throw_assert(len < MAX_MSG_LEN, "msg length is too long."<<len);

    YY::Msg _msg;
    _msg.len = len;
    memcpy(_msg.body, pbuf, len);

    if(m_nRoleType == RT_LISTEN)
    {
        Append(&m_sharemem->svrwrite, &_msg);
    }
    else
    {
        Append(&m_sharemem->svrread, &_msg);
    }
}

bool MMQ_ShareMem::EventLoop(float sec)
{
    if (m_nRoleType == RT_UNKNOWN)
    {
        return false;
    }

    if(m_nRoleType == RT_LISTEN)
    {
        // 判断是否有消息可读
        if (!IsEmpty(&m_sharemem->svrread))
        {
            YY::Msg _msg;
            Pop(&m_sharemem->svrread, &_msg);

            if (m_CBFunc_Msg)
            {
                m_CBFunc_Msg(INVALID_CONN_ID, _msg.body, _msg.len);
            }

            return true;
        }
    }
    else
    {
        // 判断是否有消息可读
        if (!IsEmpty(&m_sharemem->svrwrite))
        {
            YY::Msg _msg;
            Pop(&m_sharemem->svrwrite, &_msg);

            if (m_CBFunc_Msg)
            {
                m_CBFunc_Msg(INVALID_CONN_ID, _msg.body, _msg.len);
            }

            return true;
        }
    }

    return false;
}

void MMQ_ShareMem::Append(MsgQueue* pMsgQueue, const YY::Msg* pmsg)
{
    // 获得下一个nwriteindex
    int nnewwriteindex = pMsgQueue->nwriteindex + 1;
    if(MAX_SB_MSG_QUEUE_LEN == nnewwriteindex)
    {
        nnewwriteindex = 0;
    }

    // 判断nwriteindex是否可写
    throw_assert(nnewwriteindex != pMsgQueue->nreadindex, "cannot write, space is full.");

    // 写消息
    memcpy(&pMsgQueue->msg_queue[pMsgQueue->nwriteindex], pmsg, sizeof(YY::Msg));
    pMsgQueue->nwriteindex = nnewwriteindex;
}

bool MMQ_ShareMem::IsEmpty(MsgQueue* pMsgQueue)
{
    if (pMsgQueue->nwriteindex == pMsgQueue->nreadindex)
        return true;

    return false;
}

void MMQ_ShareMem::Pop(MsgQueue* pMsgQueue, YY::Msg* pmsg)
{
    throw_assert(!IsEmpty(pMsgQueue), "msg queue is empty.");

    // 获得下一个nreadindex
    int nnewreadindex = pMsgQueue->nreadindex+1;
    if(MAX_SB_MSG_QUEUE_LEN == nnewreadindex)
    {
        nnewreadindex = 0;
    }

    // 读消息
    memcpy(pmsg, &pMsgQueue->msg_queue[pMsgQueue->nreadindex], sizeof(YY::Msg));
    pMsgQueue->nreadindex = nnewreadindex;
}

NS_YY_END
