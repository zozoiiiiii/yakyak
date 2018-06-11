#include "conn_manager.h"
#include "core/inc/yy_exception.h"

NS_YY_BEGIN

struct ConnInfoEx
{
    ConnInfo ci;
    int serial;                //序列号，每次新连接新生成的序列号，session key的效果
};

ConnManager::ConnManager(int size):m_total_size(size), m_inuse_size(0), m_serial(1000)
{
    m_conn_array=new ConnInfoEx[size];
    memset(m_conn_array, 0, size*sizeof(ConnInfoEx));
}

ConnManager::~ConnManager()
{
    if(m_conn_array!=NULL)
        delete []m_conn_array;
}

CONN_ID ConnManager::Create(int sd)
{
    CONN_ID new_CONN_ID;

    //多一些空间来判断
    throw_assert(m_inuse_size+10 < m_total_size, "达到最大连接数:"<< m_total_size);

    int index=0;

    if(m_conn_freelist.size() > 0)
    {
        //如果freelist有可用连接
        index=m_conn_freelist.top();
        m_conn_freelist.pop();
    }
    else
    {
        //如果freelist为空，则大数组已经分配出去的连接槽肯定是连续的
        index=m_inuse_size;
    }


    //每次连接都不一样的serial
    m_serial++;

    //服务器一直开着, 不断有连接和断开, 所以会有超过int最大值的情况, 此处用一个除余来安全处理
    m_serial = m_serial % m_total_size;

    ConnInfoEx* cie=&m_conn_array[index];
    cie->ci.socket=sd;
    cie->serial=m_serial;

    CONNIDINFO tmp(index, cie->serial);
    new_CONN_ID = tmp.nDummy64;

    //在函数的最后加1，而不是函数开头，有可能函数中间异常退出，而此时m_inuse_size已累加
    m_inuse_size++;
    return new_CONN_ID;
}

void ConnManager::Destroy(CONN_ID connID)
{
    CONNIDINFO conn(connID);
    int index=conn.nIdent;
    int serial=conn.nSerial;
    throw_assert(index < m_total_size, "index:"<<index<<" is out of range:" << m_total_size);

    ConnInfoEx* pConn=&m_conn_array[index];
    throw_assert(pConn->serial == serial && pConn->ci.socket != 0,"连接已断开");

    //清空
    memset(pConn, 0, sizeof(ConnInfoEx));

    m_conn_freelist.push(index);
    m_inuse_size--;
}

ConnInfo* ConnManager::Get(CONN_ID connID)
{
    CONNIDINFO conn(connID);
    int index=conn.nIdent;
    int serial=conn.nSerial;
    throw_assert(index < m_total_size, "index:"<<index<<" is out of range:" << m_total_size);

    ConnInfoEx* pConn=&m_conn_array[index];
    throw_assert(pConn->serial ==serial && pConn->ci.socket != 0, "连接已断开");
    return &pConn->ci;
}

void ConnManager::GetAll(std::vector<CONN_ID>& v)
{
    for(int i=0; i<m_total_size; i++)
    {
        ConnInfoEx* pConn=&m_conn_array[i];
        if(pConn->ci.socket==0)
            continue;

        //封装成64位
        CONNIDINFO tmp(i, pConn->serial);
        v.push_back(tmp.nDummy64);

        //已经找到所有在使用的连接, 则直接返回.
        if(v.size()==m_inuse_size)
            break;
    }
}

NS_YY_END