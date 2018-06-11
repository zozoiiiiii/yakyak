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

    // �ͻ������ӻ��߷���˼���
    // tcp��Ӧ��addrΪ 127.0.0.1:5001; shm��Ӧ��addrΪ �����ڴ�����
    virtual void Connect(const char* paddr);

    // �ر�����
    virtual void Close();

    // false��ʾû����Ϣ��true��ʾ����Ϣ����
    virtual bool EventLoop(float sec);

    // ע�����Ȥ����Ϣ
    typedef fastdelegate::FastDelegate1<const YY::VarList&> CB_OnMsg;
    virtual void RegMsg(int id, CB_OnMsg func);
    virtual void SendMsg(int msg_id, const YY::VarList& vl = YY::VarList());
private:
    void OnCon(CONN_ID id, const char* ip, int port);
    void OnDiscon(CONN_ID id, const char* ip, int port);
    void OnRead(CONN_ID id, const char* buf, int len);
private:
    std::map<int, CB_OnMsg> m_msg_dispatch;                // ��Ϣ�ַ�
    YY::MMQ* m_mq;
    CONN_ID m_server;
};
