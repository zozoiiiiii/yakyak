/************************************************************************/
/*
@author:    junliang
@brief:     mmqʹ������
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

    // �ͻ������ӻ��߷���˼���
    // tcp��Ӧ��addrΪ 127.0.0.1:5001; shm��Ӧ��addrΪ �����ڴ�����
    virtual void Listen(const char* paddr){m_mq->Listen(paddr);}

    // �ر�����
    virtual void Close(CONN_ID id){m_mq->Close(id);}

    // false��ʾû����Ϣ��true��ʾ����Ϣ����
    virtual bool EventLoop(float sec){return m_mq->EventLoop(sec);}

    // ע�����Ȥ����Ϣ
    typedef fastdelegate::FastDelegate2<CONN_ID, const YY::VarList&> CB_OnMsg;
    virtual void RegMsg(int id, CB_OnMsg func);
    virtual void SendMsg(CONN_ID id, int msg_id, const YY::VarList& vl = YY::VarList());
private:
    void OnCon(CONN_ID id, const char* ip, int port);
    void OnDiscon(CONN_ID id, const char* ip, int port);
    void OnRead(CONN_ID id, const char* buf, int len);
private:
    std::map<int, CB_OnMsg> m_msg_dispatch;                // ��Ϣ�ַ�
    YY::MMQ* m_mq;
};
