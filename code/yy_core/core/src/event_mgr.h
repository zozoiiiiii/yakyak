/************************************************************************/
/* 
@author:    junliang
@brief:     event manager
@time:      2017-5-18
*/
/************************************************************************/
#pragma once

#include "core/inc/yy_event_mgr.h"


NS_YY_BEGIN
typedef std::map<std::string, std::vector<EventDelegate> > MAP_EVENT;
typedef std::map<YY_OBJECTID, MAP_EVENT> MAP_SENDER;

class EventMgr : public IEventMgr
{
    YY_BEGIN(EventMgr, BaseObject);
    YY_END
public:
    typedef std::map<std::string, std::vector<EventDelegate> > MAP_EVENT;
    typedef std::map<YY_OBJECTID, MAP_EVENT> MAP_SENDER;

    virtual int Invoke(YY_OBJECTID sender, const char* eventname, const YY::VarList& arg=YY::VarList());

    virtual void Connect(YY_OBJECTID sender, const char* eventname, EventDelegate cb_fun);
	virtual void Disconnect(YY_OBJECTID sender, const char* eventname, EventDelegate cb_fun);
	virtual void Disconnect(YY_OBJECTID sender);

    virtual void ConnectGlobals(EventDelegate recv_callback);
    virtual void DisconnectGlobals(EventDelegate recv_callback);

    virtual void ConnectGlobals(const char* eventname, EventDelegate recv_callback);
    virtual void DisconnectGlobals(const char* eventname, EventDelegate recv_callback);

private:
    inline void Invoke(std::vector<EventDelegate>& delegates,  const char* name, const YY::VarList& arg)
    {
        size_t cnt = delegates.size();
        for(size_t i = 0; i < cnt; i++)
        {
            delegates[i](name, arg);
        }
    }

private:
    MAP_EVENT m_globals;
    MAP_SENDER m_senders;
};

NS_YY_END