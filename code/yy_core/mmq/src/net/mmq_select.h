/************************************************************************/
/*
@author:    junliang
@brief:     select(windows, linux)
@time:      20140716
*/
/************************************************************************/
#pragma once


#include "socket_wrap.h"
#include"mmq/inc/yy_mmq.h"
#include "conn_manager.h"
NS_YY_BEGIN

class MMQ_Select : public YY::MMQ
{
public:
    MMQ_Select(CB_OnCon con, CB_OnDiscon discon, CB_OnRead read);
    virtual ~MMQ_Select();
    virtual CONN_ID Connect(const char* paddr);
    virtual void Listen(const char* paddr);
    virtual void Send(CONN_ID connID, const char* buf, int len);
    virtual void Close(CONN_ID connID);
    virtual bool EventLoop(float sec);

private:
    int InitSets();
    void OnRead(CONN_ID connID);
    void OnWrite(CONN_ID connID);
private:
    fd_set read_set_;        //������
    fd_set write_set_;        //д����
    int m_listen_sd;

    //�¼��ӿ�
    CB_OnCon m_CBFunc_Con;
    CB_OnDiscon m_CBFunc_Discon;
    CB_OnRead m_CBFunc_Msg;

    //���ӹ���
    ConnManager m_conn_manager;
};

NS_YY_END