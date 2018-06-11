#include "conn_manager.h"
#include "core/inc/yy_exception.h"

NS_YY_BEGIN

struct ConnInfoEx
{
    ConnInfo ci;
    int serial;                //���кţ�ÿ�������������ɵ����кţ�session key��Ч��
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

    //��һЩ�ռ����ж�
    throw_assert(m_inuse_size+10 < m_total_size, "�ﵽ���������:"<< m_total_size);

    int index=0;

    if(m_conn_freelist.size() > 0)
    {
        //���freelist�п�������
        index=m_conn_freelist.top();
        m_conn_freelist.pop();
    }
    else
    {
        //���freelistΪ�գ���������Ѿ������ȥ�����Ӳۿ϶���������
        index=m_inuse_size;
    }


    //ÿ�����Ӷ���һ����serial
    m_serial++;

    //������һֱ����, ���������ӺͶϿ�, ���Ի��г���int���ֵ�����, �˴���һ����������ȫ����
    m_serial = m_serial % m_total_size;

    ConnInfoEx* cie=&m_conn_array[index];
    cie->ci.socket=sd;
    cie->serial=m_serial;

    CONNIDINFO tmp(index, cie->serial);
    new_CONN_ID = tmp.nDummy64;

    //�ں���������1�������Ǻ�����ͷ���п��ܺ����м��쳣�˳�������ʱm_inuse_size���ۼ�
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
    throw_assert(pConn->serial == serial && pConn->ci.socket != 0,"�����ѶϿ�");

    //���
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
    throw_assert(pConn->serial ==serial && pConn->ci.socket != 0, "�����ѶϿ�");
    return &pConn->ci;
}

void ConnManager::GetAll(std::vector<CONN_ID>& v)
{
    for(int i=0; i<m_total_size; i++)
    {
        ConnInfoEx* pConn=&m_conn_array[i];
        if(pConn->ci.socket==0)
            continue;

        //��װ��64λ
        CONNIDINFO tmp(i, pConn->serial);
        v.push_back(tmp.nDummy64);

        //�Ѿ��ҵ�������ʹ�õ�����, ��ֱ�ӷ���.
        if(v.size()==m_inuse_size)
            break;
    }
}

NS_YY_END