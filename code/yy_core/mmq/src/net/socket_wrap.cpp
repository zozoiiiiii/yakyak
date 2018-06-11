#include "socket_wrap.h"
#include "core/inc/yy_exception.h"
#include "util/inc/yy_string.h"
#include <vector>


void SW_Open()
{
#ifdef WIN32
    //����winsock�汾����Ϣ
    WSADATA wsaData;

    //׼�����ص�Winsock��İ汾�ţ�Ŀǰwin32ƽ̨��Winsock2������°汾��2.2
    WORD wVersionRequested=MAKEWORD(2,2);

    int err=WSAStartup(wVersionRequested, &wsaData);
    throw_assert(err == 0, "WSAStartup error.");
#endif
}

void SW_Close()
{
#ifdef WIN32
    //ж��winsock2�⣬���ͷ���Դ
    WSACleanup();
#endif
}

int SW_CreateSocket()
{
    //create socket
    int new_socket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifdef WIN32
    throw_assert(new_socket != INVALID_SOCKET, "create socket error.");
#else
    throw_assert(new_socket !=-1, "create socket error.");
#endif

    return new_socket;
}

void SW_CloseSocket(int sd)
{
#ifdef WIN32
    closesocket(sd);
#else
    close(sd);
#endif
}

int SW_Listen(const char* ip, int port)
{
    throw_assert(NULL != ip, "ip��ַΪ��");

    int listen_sd = SW_CreateSocket();

    sockaddr_in service;
    service.sin_family = AF_INET;

    //���ַ���ת����32λ�������ֽ���
    service.sin_addr.s_addr = inet_addr(ip);

    //��16λ�˿ںŴ������ֽ���ת���������ֽ���
    service.sin_port = htons(port);

    throw_assert(::bind( listen_sd, (SOCKADDR*) &service, sizeof(service)) != SOCKET_ERROR, "��socket����");

     //����Ϊ������ģʽ
    u_long iMode = 1;
    throw_assert(ioctlsocket(listen_sd, FIONBIO, &iMode) != SOCKET_ERROR, "���÷���������");

    throw_assert( ::listen(listen_sd, 5) != SOCKET_ERROR,  "����socket����");

    return listen_sd;
    
    //�رշ��ͻ���,������listen֮ǰ����,����wsasend��ʱ��wsabufָ���û����������ڴ�������
    //int nZero=0;
    //assert(setsockopt(m_listen_sd, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero)) != SOCKET_ERROR);
    //assert( ::listen(m_listen_sd, 5) != SOCKET_ERROR );

    /*
    //�ر����ӵ�ʱ�򣬷��ͻ������ͽ��ܻ������е��������ݶ�����, time_wait�����Ե��
    LINGER lingerStruct;
    lingerStruct.l_onoff = 1;
    lingerStruct.l_linger = 0;

    nRet = setsockopt(m_listen_sd, SOL_SOCKET, SO_LINGER,
    (char *)&lingerStruct, sizeof(lingerStruct) );
    if( nRet == SOCKET_ERROR )
    {
        THROWEXCEPT<<"setsockopt(SO_LINGER) failed: %d\n", WSAGetLastError());
    }*/
}

//����ͻ��˺ͷ������ͬһ�������������connectͨ�������������ӣ�
//���򷵻�EINPROGRESS, ��ʾ���ӽ����Ѿ�����������δ��ɣ��Ѿ������tcp��·���ּ�������
//������connect��ֲ�Բ��ã�����һ�㶼�ǽ��̸�������ʱ�����Ĳ�����������������ʽ�����ܻ�������ʮ�룬Ҳ�ǿ��Ե�
int SW_ConnectTo(const char* ip, int port)
{
    int client_socket=SW_CreateSocket();

    //connect to server
    sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;

#ifdef WIN32
    server_addr.sin_addr.s_addr = inet_addr(ip);
#else
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
#endif
    server_addr.sin_port=htons(port);
    if(connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) ==-1 )
    {
        SW_CloseSocket(client_socket);
        return INVALID_SOCKET;
    }

    //����Ϊ������ģʽ
    u_long iMode = 1;  //non-blocking mode is enabled.
    if(ioctlsocket(client_socket, FIONBIO, &iMode) == SOCKET_ERROR)
    {
        SW_CloseSocket(client_socket);
        return INVALID_SOCKET;
    }

    return client_socket;
}


//��ʹ����select�жϣ�Ҳ�п�����select�ɹ��󣬿ͻ��˶Ͽ����ӣ�����accept����������Ҫ�÷�����io
bool SW_Accept(int listen_sd, int& new_sd)
{
    sockaddr_in clientAddr;
    int nClientLen = sizeof(sockaddr_in);

    //������accept
    new_sd=(int)::accept(listen_sd, (sockaddr*)&clientAddr, &nClientLen);
    if(new_sd == INVALID_SOCKET)
    {
        int err_num=WSAGetLastError();
        if(err_num==WSAEWOULDBLOCK)
            return false;

        throw_assert(false, "accept socket erro. errno:"<< err_num);
    }

    //�������ӽ��������óɷ�������
    u_long iMode = 1;  //non-blocking mode is enabled.
    if(ioctlsocket(new_sd, FIONBIO, &iMode) == SOCKET_ERROR)
    {
        SW_CloseSocket(new_sd);
        throw_assert(false, "set no block mode erro. errno:"<< WSAGetLastError());
    }

    return true;
}


void parse_addr(const char* addr, std::string& ip, int& port)
{
    std::vector<std::string> infos;
    YY::Split(infos, addr, ":");
    throw_assert(infos.size() == 2,  "addr in invalid.");

    ip = infos[0];
    std::string str_port = infos[1];
    port = YY::StrToInt32(str_port.c_str());
}


void parse_one_msg(CONN_ID id, const char* buf, int len, int& transfer_size, CB_OnRead func)
{
    int new_transfer_size=0;
    int new_len;
    while(true)
    {
        new_len=len-new_transfer_size;

        //��Ϣͷ�Ƿ�����
        if(new_len < sizeof(int))
            break;

        const char* new_buf=buf+new_transfer_size;

        //��������Э�飬�ж�һ��������Ϣ
        const YY::Msg* test_msg = (const YY::Msg*)new_buf;
        throw_assert(test_msg->len <= MAX_MSG_LEN, "��Ϣ�����쳣���Ͽ�����");

        //δ��һ��������Ϣ
        int total_msg_len= sizeof(int) + test_msg->len;
        if(new_len<total_msg_len)
            break;

        //���ٰ���һ��������Ϣ
        func(id, new_buf+sizeof(int), total_msg_len-sizeof(int));

        new_transfer_size += total_msg_len;
    }

    transfer_size=new_transfer_size;
}