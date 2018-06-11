/************************************************************************/
/* 
@author:    junliang
@brief:     msg queue
@time:      11/15/2016
*/
/************************************************************************/
#pragma once

#include <map>
#include "core/inc/yy_macros.h"
#include "core/inc/FastDelegate.h"
#include "../inc/yy_msg.h"

#define MAX_CONN_NUM 1024

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

#undef CONN_ID
#define CONN_ID uint64

#undef INVALID_CONN_ID
#define INVALID_CONN_ID 0

typedef fastdelegate::FastDelegate3<CONN_ID, const char*, int> CB_OnCon;
typedef fastdelegate::FastDelegate3<CONN_ID, const char*, int> CB_OnDiscon;
typedef fastdelegate::FastDelegate3<CONN_ID, const char*, int> CB_OnRead;

NS_YY_BEGIN
enum MQ_TYPE
{
    MT_UNKNOWN = -1,
    MT_SHM,                // share memory
    MT_TCP_SELECT,        // tcp select
    MT_TCP_SUPER,        // win:iocp; linux:epoll
};

enum ROLE_TYPE
{
    RT_UNKNOWN = 0,
    RT_CONNECT,         // client
    RT_LISTEN,          // svr
};

class MMQ
{
public:
    virtual ~MMQ(){};
    virtual int GetRoleType(){ return m_nRoleType; }

    // tcp addr: 127.0.0.1:5001; shm addr: share memory name.
    virtual CONN_ID Connect(const char* pAddr)=0;
    virtual void Listen(const char* pAddr)=0;
    virtual void Close(CONN_ID id)=0;
    virtual void Send(CONN_ID id, const char* pbuf, int len) = 0;

    // if has msg, return true
    virtual bool EventLoop(float sec) = 0;
protected:
    int m_nRoleType;
};

MMQ* CreateMQ(int nType, CB_OnCon con, CB_OnDiscon discon, CB_OnRead read);

NS_YY_END