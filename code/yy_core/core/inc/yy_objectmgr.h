/************************************************************************/
/*
@author:    junliang
@brief:     BaseObject manager
@time:      20160120
*/
/************************************************************************/
#pragma once

#include <string>
#include <map>
#include <vector>
#include "yy_object.h"
#include "yy_reflection_mgr.h"
#include "yy_event_mgr.h"
#include "yy_log.h"

NS_YY_BEGIN

enum EventTypeInner
{
	EVENTTYPE_INNER_BEGIN=0,
	EVENTTYPE_INPUT,
	EVENTTYPE_INNER_END=100,
};

class IObjectMgr
{
public:
    virtual ~IObjectMgr(){}

	virtual BaseObject* BeginCreate(const std::string& strClassName) = 0;
	virtual void EndCreate(BaseObject* pObject, const VariantMap& args = VariantMap()) = 0;
    virtual BaseObject* Create(const std::string& strClassName, const VariantMap& args=VariantMap()) = 0;
    virtual void Destroy(YY_OBJECTID id) = 0;
	virtual YY::Log* GetLog() = 0;
    virtual BaseObject* Find(YY_OBJECTID id)=0;
    virtual BaseObject* Get(YY_OBJECTID id)=0;

    virtual void SetGlobal(const char* name, YY::Var val)=0;
	virtual YY::Var FindGlobal(const char* name) = 0;
	virtual BaseObject* FindGlobalObject(const char* name) = 0;

    // excute
    virtual void AddExcute(BaseObject* pBaseObject) = 0;
    virtual void RemoveExcute(BaseObject* pBaseObject) = 0;
    virtual void OnExcute(float sec) = 0;

	virtual IEventMgr* GetEventMgr() = 0;

    // reflection.
    virtual IReflectionMgr* GetReflectionMgr() = 0;

	// input msg
	virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2) = 0;
	virtual void AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id) = 0;
	virtual void RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id) = 0;
};


//interface
#define LOG_ERR(pObjMgr,Msg)          pObjMgr->GetLog()->LogData((int)Log::LL_ERROR, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
#define LOG_TRACE(pObjMgr,Msg)        pObjMgr->GetLog()->LogData((int)Log::LL_TRACE, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
#define LOG_DEBUG(pObjMgr,Msg)        pObjMgr->GetLog()->LogData((int)Log::LL_DEBUG, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
NS_YY_END

