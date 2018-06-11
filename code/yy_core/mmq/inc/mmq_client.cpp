#include "mmq_client.h"


MMQClient::MMQClient(){}
MMQClient::~MMQClient(){}


void MMQClient::Init(int nType)
{
    m_mq = YY::CreateMQ(nType, fastdelegate::MakeDelegate(this, &MMQClient::OnCon),
        fastdelegate::MakeDelegate(this, &MMQClient::OnDiscon),
        fastdelegate::MakeDelegate(this, &MMQClient::OnRead));
}

void MMQClient::Connect(const char* paddr)
{
    m_server = m_mq->Connect(paddr);
}

void MMQClient::Close()
{
    m_mq->Close(m_server);
}

bool MMQClient::EventLoop(float sec)
{
    if(m_mq)
        return m_mq->EventLoop(sec);

    return false;
}

void MMQClient::RegMsg(int id, CB_OnMsg func)
{
    throw_assert(m_mq!=NULL, "not inited.");
    std::map<int, CB_OnMsg>::iterator itor = m_msg_dispatch.find(id);
    throw_assert(itor == m_msg_dispatch.end(), "same key already exist.");

    if (NULL != func)
    {
        m_msg_dispatch[id] = func;
    }
}

void MMQClient::SendMsg(int nMsgID, const YY::VarList& vl)
{
    throw_assert(m_mq!=NULL, "not inited.");
    std::string body = vl.SerializeTo();
    MsgBody mm;
    mm.id = nMsgID;
    throw_assert(body.length() + sizeof(int) < MAX_MSG_LEN,  "msg length is too long.");

    memcpy(mm.body, body.c_str(), body.length());

    const char* buf = (const char*)&mm;
    m_mq->Send(m_server, buf, (int)body.length()+sizeof(int));
}


void MMQClient::OnCon(CONN_ID id, const char* ip, int port)
{}
void MMQClient::OnDiscon(CONN_ID id, const char* ip, int port)
{}


void MMQClient::OnRead(CONN_ID id, const char* buf, int len)
{
    throw_assert(id == m_server, "recv msg from error server.");
    MsgBody* mh = (MsgBody*)buf;
    int msg_id = mh->id;
    std::map<int, CB_OnMsg>::iterator it = m_msg_dispatch.find(msg_id);
    throw_assert(m_msg_dispatch.end() != it, "not support key:" << msg_id);

    YY::VarList VarList;
    VarList.ParseFrom( std::string(mh->body, len-sizeof(int)) );
    it->second(VarList);
}
