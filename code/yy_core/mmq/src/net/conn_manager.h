/************************************************************************/
/*
@author:  junliang
@brief:   网络模块内部的连接管理
@time:    20130603
*/
/************************************************************************/
#ifndef _CONNECT_MANAGER_HPP_
#define _CONNECT_MANAGER_HPP_

#include "conn.h"
#include "socket_wrap.h"
#include <stack>
#include <vector>


NS_YY_BEGIN

// conn id 拼装
struct CONNIDINFO
{
    CONNIDINFO(uint32 ident, uint32 serial)
    {
        nIdent = ident;
        nSerial = serial;
    }

    CONNIDINFO(uint64 connID)
    {
        nDummy64 = connID;
    }

    union
    {
        struct
        {
            uint32    nIdent;
            uint32    nSerial;
        };
        uint64 nDummy64;
    };
};

struct ConnInfoEx;
class ConnManager
{
public:
    ConnManager(int size=MAX_CONN_NUM);
    ~ConnManager();

    //获得一个连接, 初始化socket
    CONN_ID Create(int sd);

    //删除一个连接
    void Destroy(CONN_ID connID);

    //获得连接信息
    ConnInfo* Get(CONN_ID connID);

    //获得所有连接
    void GetAll(std::vector<CONN_ID>& v);
private:
    int m_inuse_size;                    //在使用的大小
    int m_total_size;                    //总大小
    ConnInfoEx* m_conn_array;
    int m_serial;                        //防止一个连接断开后，新连接占用， 然后旧连接的消息发来
    std::stack<int> m_conn_freelist;    //释放的空闲连接槽， 提高效率
};

NS_YY_END
#endif