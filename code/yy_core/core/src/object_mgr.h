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
	virtual BaseObject* BeginCreate(const std::string& strClassName);
	virtual void EndCreate(BaseObject* pObject, const VariantMap& args=VariantMap());
    virtual BaseObject* Create(const std::string& strClassName, const VariantMap& args=VariantMap());
	virtual void Destroy(YY_OBJECTID id);
	virtual YY::Log* GetLog();
    virtual BaseObject* Find(YY_OBJECTID id);
    virtual BaseObject* Get(YY_OBJECTID id);

    virtual void SetGlobal(const char* name, YY::Var val);
	virtual YY::Var FindGlobal(const char* name);
	virtual BaseObject* FindGlobalObject(const char* name);

    // excute
    virtual void AddExcute(BaseObject* pBaseObject);
    virtual void RemoveExcute(BaseObject* pBaseObject);
    virtual void OnExcute(float sec);

	virtual IEventMgr* GetEventMgr() { return m_pEventMgr; }
    // input msg
    //virtual void AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
    //virtual void RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
    //virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2);

    // reflection
    virtual IReflectionMgr* GetReflectionMgr(){return m_pReflectionMgr;}

    //void SetFieldVal(BaseObject* pBaseObject, const char* prop_name, const void* val);
    //void GetFieldVal(BaseObject* pBaseObject, const char* prop_name, void* val);

	virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2);
	virtual void AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
	virtual void RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
private:
    IReflectionMgr* m_pReflectionMgr;
    std::map<YY_OBJECTID, BaseObject*> m_pEntities;
    std::map<std::string, YY::Var> m_globalVars;
    std::vector<YY_OBJECTID> m_pExcute;
	IEventMgr* m_pEventMgr;
	InputMgr* m_pInputMgr;
    sint64 m_BaseObject_id_seed;
};

NS_YY_END