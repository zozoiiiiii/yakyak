/************************************************************************/
/*
@author:  junliang
@brief:   ����ģ���ڲ������ӹ���
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

// conn id ƴװ
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

    //���һ������, ��ʼ��socket
    CONN_ID Create(int sd);

    //ɾ��һ������
    void Destroy(CONN_ID connID);

    //���������Ϣ
    ConnInfo* Get(CONN_ID connID);

    //�����������
    void GetAll(std::vector<CONN_ID>& v);
private:
    int m_inuse_size;                    //��ʹ�õĴ�С
    int m_total_size;                    //�ܴ�С
    ConnInfoEx* m_conn_array;
    int m_serial;                        //��ֹһ�����ӶϿ���������ռ�ã� Ȼ������ӵ���Ϣ����
    std::stack<int> m_conn_freelist;    //�ͷŵĿ������Ӳۣ� ���Ч��
};

NS_YY_END
#endif