#include "win_iocp.h"
#include <string>
#include "core/inc/yy_exception.h"

NS_YY_BEGIN
Win_IOCP::Win_IOCP(CB_OnCon con, CB_OnDiscon discon, CB_OnRead read)
{
    m_CBFunc_Con = con;
    m_CBFunc_Discon = discon;
    m_CBFunc_Msg = read;

    m_listen_sd = INVALID_SOCKET;
    SW_Open();

    //����iocp���
    m_iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    throw_assert(NULL!=m_iocp_handle, "CreateIoCompletionPort error.");
}

Win_IOCP::~Win_IOCP()
{
    //������
    if(m_iocp_handle)
    {
        PostQueuedCompletionStatus(m_iocp_handle, 0, 0, NULL);
        CloseHandle(m_iocp_handle);
        m_iocp_handle = NULL;
    }

    SW_Close();
}
void Win_IOCP::UpdateCompletionPort(SOCKET sd, CONN_ID connID)
{
    //�����Ӿ������ɶ˿ڰ�
    m_iocp_handle = CreateIoCompletionPort((HANDLE)sd, m_iocp_handle, 0, 0);
    throw_assert( m_iocp_handle != NULL, "�����Ӿ������ɶ˿ڰ�ʧ��");
}

void Win_IOCP::PostRecvRequest(CONN_ID connID)
{
    ConnInfo* p=m_conn_manager.Get(connID);

    //Ͷ�ݶ�����
    IOContext* io_cxt=NewIOCxt(ClientIoRead, connID);

    DWORD dwRecvNumBytes = 0;
    DWORD dwFlags = 0;

    WSABUF wsabuf;
    wsabuf.buf=p->r_buf + p->r_size;
    wsabuf.len=sizeof(p->r_buf) - p->r_size;


    int nRet = WSARecv(p->socket, &wsabuf, 1, &dwRecvNumBytes, &dwFlags, &io_cxt->overlapped, NULL);
    if( nRet == SOCKET_ERROR)
    {
        int nErrNo=WSAGetLastError();
        if(ERROR_IO_PENDING == nErrNo)
            return;

        //WSAECONNRESET, �Զ˹ر�����; WSAEWOULDBLOCK, �����ж��ش���ͳһ�رո�����
        Close(connID);
    }
}

void Win_IOCP::PostSendRequest(CONN_ID connID)
{
    ConnInfo* socket_p=m_conn_manager.Get(connID);

    DWORD dwSendNumBytes = 0;
    DWORD dwFlags = 0;

    IOContext* io_p=NewIOCxt(ClientIoWrite, connID);

    WSABUF wsabuf;
    wsabuf.buf=socket_p->w_buf;
    wsabuf.len=socket_p->w_size;

    int nRet = WSASend(socket_p->socket, &wsabuf, 1, &dwSendNumBytes, dwFlags, (LPWSAOVERLAPPED)io_p, NULL);
    if( nRet == SOCKET_ERROR)
    {
        int nErrNo=WSAGetLastError();
        if(ERROR_IO_PENDING == nErrNo)
            return;

        //WSAECONNRESET, �Զ˹ر�����; WSAEWOULDBLOCK, �����ж��ش���ͳһ�رո�����
        Close(connID);
    }
}


CONN_ID Win_IOCP::Connect(const char* addr)
{
    std::string ip;
    int port;
    parse_addr(addr, ip, port);

    int socketfd=INVALID_SOCKET;

    //����ָ��ip
    socketfd =SW_ConnectTo(ip.c_str(), port);
    if(socketfd == INVALID_SOCKET)
        return INVALID_SOCKET;

    m_nRoleType = RT_CONNECT;

    //ע��
    return RegisterSocket(socketfd);
}

void Win_IOCP::Listen(const char* addr)
{
    std::string ip;
    int port;
    parse_addr(addr, ip, port);
    m_listen_sd=SW_Listen(ip.c_str(), port);
    m_nRoleType = RT_LISTEN;
}

bool Win_IOCP::OnAcceptTick()
{
    if(m_listen_sd == INVALID_SOCKET)
        return false;

    //init
    FD_ZERO(&m_read_set);

    //��Ӽ��������read array,��������ӽ���������Ӧ
    FD_SET(m_listen_sd, &m_read_set);

    //select accept event
    timeval timeout;
    timeout.tv_sec=0;        //seconds
    timeout.tv_usec=0;        //micro seconds
    int ready_socket_num = select(    0,                    //����
                                    &m_read_set,        //ָ������е�ָ��
                                    NULL,
                                    NULL,
                                    &timeout);            //��ʱ

    throw_assert(ready_socket_num != -1, "select erro.errno:"<< WSAGetLastError());

    //new connection
    if(ready_socket_num>0 && FD_ISSET(m_listen_sd, &m_read_set))
    {
        //����������ӣ����÷�����ioȥaccept,�п���ʧ��
        int sd;
        if(SW_Accept(m_listen_sd, sd))
            RegisterSocket(sd);

        return true;
    }

    return false;
}

void Win_IOCP::Send(CONN_ID connID, const char* buf, int len)
{
    throw_assert (m_nRoleType != RT_UNKNOWN, "role type is unknown.");
    throw_assert(len <= MAX_MSG_LEN, "�������������Ϣ����");

    ConnInfo* p=m_conn_manager.Get(connID);

    bool no_send_post=false;
    if(p->w_size == 0)
    {
        //֮ǰû������Ͷ��
        no_send_post=true;
    }


    // ��Ϣǰ����ϳ���
    YY::Msg m;
    m.len = len;
    memcpy(m.body, buf, len);
    const char* total_msg = (const char*)&m;
    int total_msg_len = len+sizeof(int);
    memcpy(p->w_buf+p->w_size, total_msg, total_msg_len);
    p->w_size+=total_msg_len;

    //memcpy(p->w_buf + p->w_size, buf, len);
    //p->w_size += len;

    if(no_send_post)
        PostSendRequest(connID);
}

void Win_IOCP::Close(CONN_ID connID)
{
    if (m_nRoleType == RT_UNKNOWN)
    {
        return;
    }

    ConnInfo* p=m_conn_manager.Get(connID);

    if(m_CBFunc_Discon)
        m_CBFunc_Discon(connID, p->remote_ip, p->remote_port);

    //��GetQueuedCompletionStatus����
    SW_CloseSocket(p->socket);
    m_conn_manager.Destroy(connID);
    p=NULL;

}


IOContext* Win_IOCP::NewIOCxt(IO_OPERATION type, CONN_ID connID)
{
    IOContext* io_cxt=NULL;
    if(m_iocxt_pool.size() > 0)
    {
        io_cxt=m_iocxt_pool.top();
        m_iocxt_pool.pop();
    }
    else
    {
        io_cxt=new IOContext;
    }

    memset(io_cxt, 0, sizeof(IOContext));
    io_cxt->io_operation=type;
    io_cxt->connID=connID;
    return io_cxt;
}

void Win_IOCP::DeleteIOCxt(IOContext* io_cxt)
{
    //���
    memset(io_cxt, 0, sizeof(IOContext));
    m_iocxt_pool.push(io_cxt);
}

CONN_ID Win_IOCP::RegisterSocket(int sd)
{
    CONN_ID connID=m_conn_manager.Create(sd);
    ConnInfo* p=m_conn_manager.Get(connID);

    //�󶨵���ɶ˿�
    UpdateCompletionPort(sd, connID);

    //���ip��Ϣ
    struct sockaddr_in remote;
    int remote_len = sizeof(remote);
    getpeername(sd, (struct sockaddr *)&remote, &remote_len);
    strcpy_s(p->remote_ip, sizeof(p->remote_ip), inet_ntoa(remote.sin_addr));
    p->remote_port=ntohs(remote.sin_port);

    p->socket=sd;

        //���ûص�����
    if(m_CBFunc_Con)
    {
        m_CBFunc_Con(connID, p->remote_ip, p->remote_port);
    }

    //Ͷ�ݶ�����
    PostRecvRequest(connID);
    return connID;
}


void Win_IOCP::ProcessWrite(CONN_ID connID, int size)
{
    ConnInfo* p=m_conn_manager.Get(connID);


    throw_assert(size <= p->w_size, "�������ݳ����쳣"<< size <<"::"<<p->w_size);

    //���������ݺ�鿴��û��������Ҫ������
    if(size == p->w_size)
    {
        p->w_size=0;
        return;
    }

    //�����������Ҫ���ͣ������Ͷ��
    memmove(p->w_buf, p->w_buf+size, p->w_size-size);
    p->w_size -= size;

    PostSendRequest(connID);
}

void Win_IOCP::ProcessRead(CONN_ID connID, int size)
{
    if(size>0)
    {
        ConnInfo* p=m_conn_manager.Get(connID);

        //�������ݵ���
        p->r_size += size;

        int transferred_size=0;

        //֪ͨӦ�ò�--���¼�
        if(m_CBFunc_Msg)
        {
            parse_one_msg(connID, p->r_buf, p->r_size, transferred_size, m_CBFunc_Msg);
        }

        //����������Ϣ
        if(transferred_size != 0)
        {
            memmove(p->r_buf, p->r_buf+transferred_size, transferred_size);
            p->r_size -= transferred_size;
        }
    }

    //����Ͷ�ݶ�����
    PostRecvRequest(connID);
}

bool Win_IOCP::EventLoop(float sec)
{
    if (m_nRoleType == RT_UNKNOWN)
    {
        return false;
    }

    if(m_listen_sd!=INVALID_SOCKET && OnAcceptTick())
    {
        return true;
    }

    return OnEventTick(sec);
}

bool Win_IOCP::OnEventTick(float sec)
{
    IOContext* io_context=NULL;
    ConnInfo* p=NULL;
    int notinuse=0;
    CONN_ID connID=0;

    LPOVERLAPPED* lpOverlapped = NULL;
    int nRet = 0;
    DWORD dwRecvNumBytes = 0;
    DWORD dwSendNumBytes = 0;
    DWORD dwFlags = 0;
    DWORD dwIoSize = 0;        //������ֽ���

    // ��ǰ���ǵ��̵߳ķ�ʽ�����Գ�ʱʱ��Ϊ0.���������̡߳�
    BOOL bSuccess = GetQueuedCompletionStatus(m_iocp_handle, &dwIoSize,(PDWORD_PTR)&notinuse, (LPOVERLAPPED *)&lpOverlapped, 0);
    if(!bSuccess)
    {
        DWORD err_num=GetLastError();
        if(err_num==WAIT_TIMEOUT)
        {
            //��ʱ����,˵����ʱû����������
            return false;
        }

        //NULL == lpOverlapped, ��lpNumberOfBytes and lpCompletionKey ��δ����ʼ����
        //NULL != lpOverlapped, ��lpNumberOfBytes and lpCompletionKey �б���ʼ��

        //client close socket
        //wsasend/wsarecv are not finished, and the socket is cancelled.
        if(err_num==ERROR_NETNAME_DELETED &&NULL != lpOverlapped )
        {
            io_context=(IOContext*)lpOverlapped;
            connID=io_context->connID;
            p=m_conn_manager.Get(connID);
            Close(connID);
            io_context=(IOContext*)lpOverlapped;
            DeleteIOCxt(io_context);
            return false;
        }

        //�д���ֱ�ӱ���ERROR_ABANDONED_WAIT_0
        throw_assert(false, "GetQueuedCompletionStatus error. errno:"<<err_num);
    }

    //�����˳���ʱ��,������ͨ��
    if(NULL==lpOverlapped)
    {
        return false;
    }

    //�������Ϣ�Ĵ���
    io_context=(IOContext*)lpOverlapped;
    connID=io_context->connID;
    p=m_conn_manager.Get(connID);

    //�жϾ�����Ϣ
    switch(io_context->io_operation)
    {
    case ClientIoRead:

        //conn shutdown 
        if(0 == dwIoSize)
        {
            Close(connID);
        }
        else
            ProcessRead(connID, dwIoSize);
        break;

        //send post, 0 if sending
    case ClientIoWrite:
        ProcessWrite(connID, dwIoSize);
        break;

    default:
        throw_assert(false, "should not be here.");
        break;
    }

    if(io_context)
    {
        DeleteIOCxt(io_context);
    }
    return true;
}


NS_YY_END