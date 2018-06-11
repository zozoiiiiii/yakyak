/************************************************************************/
/*
@author:    junliang
@brief:     entity manager
@time:      20160120
*/
/************************************************************************/
#pragma once

#include <string>
#include <map>
#include <vector>
#include "../inc/yy_entity.h"
#include "../inc/yy_reflection_mgr.h"

NS_YY_BEGIN

class IEntityMgr
{
public:
    IEntityMgr(){}
    virtual ~IEntityMgr(){}

    virtual Entity* Create(const std::string& strClassName) = 0;
    virtual void Destroy(YY_ENTITYID id) = 0;
    //virtual bool SerializeTo(YY::Entity* pEntity, YY::VarList& args)=0;
    virtual bool IsInstanceOf(YY::Entity* pEntity, const std::string& strClassName)=0;

    virtual Entity* Find(YY_ENTITYID id)=0;
    virtual Entity* Get(YY_ENTITYID id)=0;

    virtual void SetGlobal(const char* name, YY::Var val)=0;
    virtual YY::Var GetGlobal(const char* name)=0;
    virtual bool FindGlobal(const char* name)=0;

    // excute
    virtual void AddExcute(Entity* pEntity) = 0;
    virtual void RemoveExcute(Entity* pEntity) = 0;
    virtual void OnExcute(float sec) = 0;

    // input msg
    virtual void AddMsgProc(YY::Entity* pEntity, uint32 msg_id)=0;
    virtual void RemoveMsgProc(YY::Entity* pEntity, uint32 msg_id)=0;
    virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2)=0;  // return false cause the defualt msg process.

    // reflection.
    virtual IReflectionMgr* GetReflectionMgr() = 0;
    //virtual void SetFieldVal(Entity* pEntity, const char* prop_name, const void* val) = 0;
    //virtual void GetFieldVal(Entity* pEntity, const char* prop_name, void* val) = 0;
};

NS_YY_END
