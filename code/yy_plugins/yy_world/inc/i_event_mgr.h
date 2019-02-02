/************************************************************************/
/* 
@author:    junliang
@brief:     event manager
@time:      2017-5-18
*/
/************************************************************************/
#pragma once

#include "yy_core.h"

typedef fastdelegate::FastDelegate2<const char*, const YY::VarList&, void> EventDelegate;

class IEventMgr : public YY::BaseObject
{
public:
    typedef std::map<std::string, std::vector<EventDelegate> > MAP_EVENT;
    typedef std::map<YY_OBJECTID, MAP_EVENT> MAP_SENDER;

    virtual int Invoke(YY_OBJECTID sender, const char* eventname, const YY::VarList& arg=YY::VarList()) = 0;

    virtual void Connect(YY_OBJECTID sender, const char* eventname, EventDelegate cb_fun) = 0;
    virtual void Disconnect(YY_OBJECTID sender, const char* eventname, EventDelegate cb_fun) = 0;

    virtual void ConnectGlobals(EventDelegate recv_callback) = 0;
    virtual void DisconnectGlobals(EventDelegate recv_callback) = 0;

    virtual void ConnectGlobals(const char* eventname, EventDelegate recv_callback) = 0;
    virtual void DisconnectGlobals(const char* eventname, EventDelegate recv_callback) = 0;
};