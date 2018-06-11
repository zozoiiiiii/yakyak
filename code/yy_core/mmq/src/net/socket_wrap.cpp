#include "socket_wrap.h"
#include "core/inc/yy_exception.h"
#include "util/inc/yy_string.h"
#include <vector>


void SW_Open()
{
#ifdef WIN32
    //包含winsock版本的信息
    WSADATA wsaData;

    //准备加载的Winsock库的版本号，目前win32平台的Winsock2库的最新版本是2.2
    WORD wVersionRequested=MAKEWORD(2,2);

    int err=WSAStartup(wVersionRequested, &wsaData);
    throw_assert(err == 0, "WSAStartup error.");
#endif
}

void SW_Close()
{
#ifdef WIN32
    //卸载winsock2库，并释放资源
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
    throw_assert(NULL != ip, "ip地址为空");

    int listen_sd = SW_CreateSocket();

    sockaddr_in service;
    service.sin_family = AF_INET;

    //把字符串转换成32位的网络字节序
    service.sin_addr.s_addr = inet_addr(ip);

    //把16位端口号从主机字节序转换成网络字节序
    service.sin_port = htons(port);

    throw_assert(::bind( listen_sd, (SOCKADDR*) &service, sizeof(service)) != SOCKET_ERROR, "绑定socket错误");

     //设置为非阻塞模式
    u_long iMode = 1;
    throw_assert(ioctlsocket(listen_sd, FIONBIO, &iMode) != SOCKET_ERROR, "设置非阻塞错误");

    throw_assert( ::listen(listen_sd, 5) != SOCKET_ERROR,  "监听socket错误");

    return listen_sd;
    
    //关闭发送缓冲,必须在listen之前设置,这样wsasend的时候，wsabuf指向用户缓冲区，内存锁定。
    //int nZero=0;
    //assert(setsockopt(m_listen_sd, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero)) != SOCKET_ERROR);
    //assert( ::listen(m_listen_sd, 5) != SOCKET_ERROR );

    /*
    //关闭连接的时候，发送缓冲区和接受缓冲区中的所有数据都丢弃, time_wait错误的缘由
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

//如果客户端和服务端在同一主机，则非阻塞connect通常立即建立连接；
//否则返回EINPROGRESS, 表示连接建立已经启动但是尚未完成，已经发起的tcp三路握手继续进行
//非阻塞connect移植性不好，而且一般都是进程刚启动的时候做的操作，所以用阻塞方式，可能会阻塞几十秒，也是可以的
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

    //设置为非阻塞模式
    u_long iMode = 1;  //non-blocking mode is enabled.
    if(ioctlsocket(client_socket, FIONBIO, &iMode) == SOCKET_ERROR)
    {
        SW_CloseSocket(client_socket);
        return INVALID_SOCKET;
    }

    return client_socket;
}


//即使用了select判断，也有可能在select成功后，客户端断开连接，导致accept阻塞，所以要用非阻塞io
bool SW_Accept(int listen_sd, int& new_sd)
{
    sockaddr_in clientAddr;
    int nClientLen = sizeof(sockaddr_in);

    //非阻塞accept
    new_sd=(int)::accept(listen_sd, (sockaddr*)&clientAddr, &nClientLen);
    if(new_sd == INVALID_SOCKET)
    {
        int err_num=WSAGetLastError();
        if(err_num==WSAEWOULDBLOCK)
            return false;

        throw_assert(false, "accept socket erro. errno:"<< err_num);
    }

    //有新连接进来，设置成非阻塞的
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

        //消息头是否完整
        if(new_len < sizeof(int))
            break;

        const char* new_buf=buf+new_transfer_size;

        //根据网络协议，判断一条完整消息
        const YY::Msg* test_msg = (const YY::Msg*)new_buf;
        throw_assert(test_msg->len <= MAX_MSG_LEN, "消息长度异常，断开连接");

        //未满一条完整消息
        int total_msg_len= sizeof(int) + test_msg->len;
        if(new_len<total_msg_len)
            break;

        //至少包含一条完整消息
        func(id, new_buf+sizeof(int), total_msg_len-sizeof(int));

        new_transfer_size += total_msg_len;
    }

    transfer_size=new_transfer_size;
}