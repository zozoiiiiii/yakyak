#include "event_mgr.h"

NS_YY_BEGIN
int EventMgr::Invoke(YY_OBJECTID sender, const char* eventname, const YY::VarList& arg)
{
    MAP_EVENT::iterator itGlobal = m_globals.find(eventname);
    if ( itGlobal != m_globals.end() )
    {
        Invoke(itGlobal->second, eventname, arg); 
    }

    MAP_EVENT::iterator itGlobalAll = m_globals.find("");
    if (itGlobalAll!=m_globals.end())
    {
        Invoke(itGlobalAll->second, eventname, arg); 
    }

    if(m_senders.empty())
    {	
        return 0;
    }

    MAP_SENDER::iterator it = m_senders.find(sender);
    if(it == m_senders.end())
    {	
        return 0;
    }

    MAP_EVENT& events = it->second;
    MAP_EVENT::iterator itMap =  events.find(std::string(eventname));
    if(itMap == events.end())
    {	
        return 0;
    }

    std::vector<EventDelegate>& delegates = itMap->second;
    Invoke(itMap->second, itMap->first.c_str(), arg);

    return 1;
}

void EventMgr::Connect(YY_OBJECTID sender, const char* eventname, EventDelegate recv_callback)
{
    MAP_EVENT& e = m_senders[sender];
    std::vector<EventDelegate>& s = e[eventname];
    s.push_back(recv_callback);
}

void EventMgr::Disconnect(YY_OBJECTID sender)
{
	MAP_SENDER::iterator it = m_senders.find(sender);
	if (it == m_senders.end())
		return;

	m_senders.erase(it);
}

void EventMgr::Disconnect(YY_OBJECTID sender, const char* eventname, EventDelegate recv_callback)
{
    MAP_SENDER::iterator it = m_senders.find(sender);
    if(it == m_senders.end())
        return;

    MAP_EVENT& events = it->second;
    MAP_EVENT::iterator itMap =  events.find(std::string(eventname));
    if(itMap == events.end())
        return ;

    std::vector<EventDelegate>& delegates = itMap->second;
    for(size_t i = 0; i < delegates.size(); i++)
    {
        if(delegates[i] == recv_callback)
        {
            delegates.erase(delegates.begin() + i);
            i--;
        }
    }
}


void EventMgr::ConnectGlobals(EventDelegate recv_callback)
{
    m_globals[""].push_back(recv_callback);
}

void EventMgr::DisconnectGlobals(EventDelegate recv_callback)
{
    MAP_EVENT::iterator it = m_globals.find("");
    if(it == m_globals.end())
    {
        return;
    }

    std::vector<EventDelegate>& delegates = it->second;
    for(size_t i = 0; i < delegates.size(); i++)
    {
        if(delegates[i] == recv_callback)
        {
            delegates.erase(delegates.begin() + i);
            i--;
        }
    }
}


void EventMgr::ConnectGlobals(const char* eventname, EventDelegate recv_callback)
{
    m_globals[eventname].push_back(recv_callback);
}

void EventMgr::DisconnectGlobals(const char* eventname, EventDelegate recv_callback)
{
    MAP_EVENT::iterator it = m_globals.find(eventname);
    if(it == m_globals.end())
    {
        return;
    }

    std::vector<EventDelegate>& delegates = it->second;
    for(size_t i = 0; i < delegates.size(); i++)
    {
        if(delegates[i] == recv_callback)
        {
            delegates.erase(delegates.begin() + i);
            i--;
        }
    }
}

NS_YY_END