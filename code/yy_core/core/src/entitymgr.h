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
#include "reflection/yy_type.h"
#include "../inc/yy_entity.h"
#include "core/inc/yy_varlist.h"
#include "reflection/yy_reflection.h"
#include "../inc/yy_entitymgr.h"
#include "../inc/yy_reflection_mgr.h"
#include "input_mgr.h"


NS_YY_BEGIN
class EntityMgr : public IEntityMgr
{
public:
    EntityMgr();
    virtual ~EntityMgr(){}
    virtual Entity* Create(const std::string& strClassName);
    virtual void Destroy(YY_ENTITYID id);
    virtual bool SerializeTo(YY::Entity* pEntity, YY::VarList& args);
    virtual bool IsInstanceOf(YY::Entity* pEntity, const std::string& strClassName);

    virtual Entity* Find(YY_ENTITYID id);
    virtual Entity* Get(YY_ENTITYID id);

    virtual void SetGlobal(const char* name, YY::Var val);
    virtual YY::Var GetGlobal(const char* name);
    virtual bool FindGlobal(const char* name);

    // excute
    virtual void AddExcute(Entity* pEntity);
    virtual void RemoveExcute(Entity* pEntity);
    virtual void OnExcute(float sec);

    // input msg
    virtual void AddMsgProc(YY::Entity* pEntity, uint32 msg_id);
    virtual void RemoveMsgProc(YY::Entity* pEntity, uint32 msg_id);
    virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2);

    // reflection
    virtual IReflectionMgr* GetReflectionMgr(){return m_pReflectionMgr;}

    //void SetFieldVal(Entity* pEntity, const char* prop_name, const void* val);
    //void GetFieldVal(Entity* pEntity, const char* prop_name, void* val);
private:
    IReflectionMgr* m_pReflectionMgr;
    std::map<YY_ENTITYID, Entity*> m_pEntities;
    std::map<std::string, YY::Var> m_globalVars;
    std::vector<YY_ENTITYID> m_pExcute;
    InputMgr m_input_mgr;
    sint64 m_entity_id_seed;
};
NS_YY_END