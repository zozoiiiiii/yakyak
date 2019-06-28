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
#include "reflection/yy_type.h"
#include "../inc/yy_object.h"
#include "core/inc/yy_varlist.h"
#include "reflection/yy_reflection.h"
#include "../inc/yy_ObjectMgr.h"
#include "../inc/yy_reflection_mgr.h"
#include "input_mgr.h"
#include "core/inc/yy_event_mgr.h"

NS_YY_BEGIN
class ObjectMgr : public IObjectMgr
{
public:
    ObjectMgr();
	virtual ~ObjectMgr() {}
    virtual BaseObject* Create(const std::string& strClassName, YY_OBJECTID id = YY_INVALID_OBJECTID);
	virtual void Destroy(YY_OBJECTID id);
    virtual BaseObject* Find(YY_OBJECTID id);
    virtual BaseObject* Get(YY_OBJECTID id);

    virtual void SetGlobal(const char* name, YY::Var val);
	virtual YY::Var FindGlobal(const char* name);
	virtual BaseObject* FindGlobalObject(const char* name);

    // excute
    virtual void AddExcute(BaseObject* pBaseObject);
    virtual void RemoveExcute(BaseObject* pBaseObject);
    virtual void OnExcute(float sec);

	virtual YY::Log* GetLog();
	virtual IEventMgr* GetEventMgr() { return m_pEventMgr; }
    virtual IReflectionMgr* GetReflectionMgr(){return m_pReflectionMgr;}

	virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2);
	virtual void AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
	virtual void RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
private:
    std::map<YY_OBJECTID, BaseObject*> m_pEntities;
    std::map<std::string, YY::Var> m_globalVars;
	std::vector<YY_OBJECTID> m_pExcute;
	IReflectionMgr* m_pReflectionMgr;
	IEventMgr* m_pEventMgr;
	InputMgr* m_pInputMgr;
    sint64 m_BaseObject_id_seed;
};

NS_YY_END