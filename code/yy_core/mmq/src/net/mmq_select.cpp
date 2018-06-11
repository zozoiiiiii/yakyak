#include "mmq_select.h"
#include "core/inc/yy_exception.h"

NS_YY_BEGIN

MMQ_Select::MMQ_Select(CB_OnCon con, CB_OnDiscon discon, CB_OnRead read)
{
    m_listen_sd = INVALID_SOCKET;
    SW_Open();
    m_CBFunc_Con = con;
    m_CBFunc_Discon = discon;
    m_CBFunc_Msg = read;
}

MMQ_Select::~MMQ_Select()
{
    SW_Close();
}

CONN_ID MMQ_Select::Connect(const char* addr)
{
    std::string ip;
    int port;
    parse_addr(addr, ip, port);

    //����ָ��ip
    int socketfd =SW_ConnectTo(ip.c_str(), port);
    if(socketfd == INVALID_SOCKET)
		return INVALID_CONN_ID;

    //��¼��������Ϣ
    CONN_ID new_id=m_conn_manager.Create(socketfd);
    ConnInfo* p=m_conn_manager.Get(new_id);
    strcpy_s(p->remote_ip, sizeof(p->remote_ip), ip.c_str());
    p->remote_port=port;
    p->socket=socketfd;
    m_nRoleType = RT_CONNECT;
    return new_id;
}

void MMQ_Select::Listen(const char* addr)
{
    std::string ip;
    int port;
    parse_addr(addr, ip, port);
    m_listen_sd=SW_Listen(ip.c_str(), port);
    m_nRoleType = RT_LISTEN;
}

void MMQ_Select::Send(CONN_ID connID, const char* buf, int len)
{
    throw_assert(m_nRoleType != RT_UNKNOWN, "role type is unknown.");
    throw_assert(len <= MAX_MSG_LEN, "�������������Ϣ����");

    ConnInfo* p=m_conn_manager.Get(connID);

    // ��Ϣǰ����ϳ���
    YY::Msg m;
    m.len = len;
    memcpy(m.body, buf, len);
    const char* total_msg = (const char*)&m;
    int total_msg_len = len+sizeof(int);
    memcpy(p->w_buf+p->w_size, total_msg, total_msg_len);
    p->w_size+=total_msg_len;
}

void MMQ_Select::Close(CONN_ID connID)
{
    if (m_nRoleType == RT_UNKNOWN)
    {
        return;
    }

    ConnInfo* p=m_conn_manager.Get(connID);

    if(m_CBFunc_Discon)
        m_CBFunc_Discon(connID, p->remote_ip, p->remote_port);

    SW_CloseSocket(p->socket);

    //��������
    m_conn_manager.Destroy(connID);
}

void MMQ_Select::OnRead(CONN_ID connID)
{
    ConnInfo* p=m_conn_manager.Get(connID);
    int ret=0;
#ifdef WIN32
    ret= ::recv(p->socket, p->r_buf, MAX_MSG_LEN, 0);
    if(ret==0)
    {
        //�Ͽ�����
        Close(connID);
        return;
    }
#else
    ret=read(p->socket, p->r_buf, MAX_MSG_LEN);
#endif

    if(ret<0)
    {
        //�Ͽ�����
        Close(connID);
        return;
    }

    p->r_size+=ret;

    //���˶��ٸ��ֽ�
    int readNum=0;

    //���ûص�
    if(m_CBFunc_Msg)
        parse_one_msg(connID, p->r_buf, p->r_size, readNum, m_CBFunc_Msg);

    //���˶��ٸ��ֽ�, �������
    p->r_size-=readNum;
}

void MMQ_Select::OnWrite(CONN_ID connID)
{
    ConnInfo* p=m_conn_manager.Get(connID);

    //д��������������Ҫ����
    if(p->w_size>0)
    {
        int bytes=0;
#ifdef WIN32
        bytes=::send(p->socket, p->w_buf, p->w_size, 0);
#else
        bytes=write(p->socket, p->w_buf, p->w_size);
#endif

        if(bytes<0)
        {
            Close(connID);
            return;
        }

        p->w_size-=bytes;
    }
}

bool MMQ_Select::EventLoop(float sec)
{
    if (m_nRoleType == RT_UNKNOWN)
    {
        return false;
    }

    //��ʼ�����
    InitSets();

    timeval timeout;
    timeout.tv_sec=0;
    timeout.tv_usec=0;

    //��������socket
    int ready_socket_num = select(    0,                    //����
                                    &read_set_,            //ָ������е�ָ��
                                    &write_set_,        //ָ��д���е�ָ��
                                    NULL,
                                    &timeout);            //��ʱ

    //�����socket��״̬�����仯
    throw_assert(ready_socket_num >= 0, "MMQ_Select error.");

    if(ready_socket_num==0)
    {
        //timeout, no msg
        return false;
    }

    bool has_event = false;

    //��������
    if(FD_ISSET(m_listen_sd, &read_set_))
    {
        int new_sd=INVALID_SOCKET;
        bool ret=SW_Accept(m_listen_sd, new_sd);
        if(ret)
        {
            CONN_ID connID=m_conn_manager.Create(new_sd);
            ConnInfo* pConn=m_conn_manager.Get(connID);
            if(m_CBFunc_Con)
                m_CBFunc_Con(connID, pConn->remote_ip, pConn->remote_port);

            has_event = true;
        }
    }

    std::vector<CONN_ID> v;
    m_conn_manager.GetAll(v);
    for(size_t i=0; i<v.size(); i++)
    {
        ConnInfo* p=m_conn_manager.Get(v[i]);
        
        //���¼�
            if(FD_ISSET(p->socket, &read_set_))
            {
                OnRead(v[i]);
                has_event = true;
            }

            //д�¼�
            if(FD_ISSET(p->socket, &write_set_))
            {
                OnWrite(v[i]);
                has_event = true;
            }
    }

    return has_event;
}

int MMQ_Select::InitSets()
{
    //����������еľ���������
    FD_ZERO(&read_set_);
    FD_ZERO(&write_set_);

    std::vector<CONN_ID> v;
    m_conn_manager.GetAll(v);
    for(uint32 i=0; i<v.size(); i++)
    {
        ConnInfo* pConn=m_conn_manager.Get(v[i]);

        //������Ҫ���͵�ʱ�򣬾Ͳ��������¼�����ֹͬʱ��дͬһ��socket.
        if(pConn->w_size>0)
        {
            //��������Ҫ����, ���ȷ������ݣ��ȷ�����ټ������¼�
            FD_SET(pConn->socket, &write_set_);
        }
        else
        {
            //��ӵ�read array
            FD_SET(pConn->socket, &read_set_);
        }
    }

    if(m_listen_sd!=INVALID_SOCKET)
    {
        FD_SET(m_listen_sd, &read_set_);
    }
    return 0;
}

NS_YY_END