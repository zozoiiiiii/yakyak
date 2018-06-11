#include "mmq_svr.h"

MMQSvr::MMQSvr(){}
MMQSvr::~MMQSvr(){}

void MMQSvr::Init(int nType)
{
    m_mq = YY::CreateMQ(nType, fastdelegate::MakeDelegate(this, &MMQSvr::OnCon),
        fastdelegate::MakeDelegate(this, &MMQSvr::OnDiscon),
        fastdelegate::MakeDelegate(this, &MMQSvr::OnRead));
}

void MMQSvr::RegMsg(int id, CB_OnMsg func)
{
    std::map<int, CB_OnMsg>::iterator itor = m_msg_dispatch.find(id);
    throw_assert(itor == m_msg_dispatch.end(), "same key already exist.");

    if (NULL != func)
    {
        m_msg_dispatch[id] = func;
    }
}

void MMQSvr::SendMsg(CONN_ID id, int nMsgID, const YY::VarList& vl)
{
    if (m_mq)
    {
        std::string body = vl.SerializeTo();
        MsgBody mm;
        mm.id = nMsgID;

        throw_assert (body.length() + sizeof(int) < MAX_MSG_LEN, "msg length is too long.");
        memcpy(mm.body, body.c_str(), body.length());

        const char* buf = (const char*)&mm;
        m_mq->Send(id, buf, (int)body.length()+sizeof(int));
    }
}


void MMQSvr::OnCon(CONN_ID id, const char* ip, int port)
{}
void MMQSvr::OnDiscon(CONN_ID id, const char* ip, int port)
{}


void MMQSvr::OnRead(CONN_ID id, const char* buf, int len)
{
    MsgBody* mh = (MsgBody*)buf;
    int msg_id = mh->id;
    std::map<int, CB_OnMsg>::iterator it = m_msg_dispatch.find(msg_id);
    throw_assert(m_msg_dispatch.end() != it, "not support key:" << msg_id);

    YY::VarList VarList;
    VarList.ParseFrom( std::string(mh->body, len-sizeof(int)));
    it->second(id, VarList);
}