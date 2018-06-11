#include "entitymgr.h"
#include <algorithm>
#include "core/inc/yy_var.h"
#include "core/inc/yy_varlist.h"
#include "core/inc/yy_exception.h"
#include "reflection_mgr.h"


NS_YY_BEGIN

/* guid is not quick enough, use one int seed to instead, although id will not unique when app restart next time.
#include <Windows.h>
uint64 gen_uint64_guid()
{
    GUID id;

    if (CoCreateGuid(&id) != S_OK)
    {
        return 0;
    }

    uint64 res = (uint64)id.Data1 << 40 | (uint64)id.Data2 << 24 | (uint64)id.Data3 << 8 | (uint64)id.Data4[0] << 7
        | (uint64)id.Data4[1] << 6 | (uint64)id.Data4[2] << 5 | (uint64)id.Data4[3] << 4 | (uint64)id.Data4[4] << 3
        | (uint64)id.Data4[5] << 2 | (uint64)id.Data4[6] << 1 | (uint64)id.Data4[7];


    return res;
}


// generate guid like {9245fe4a-d402-451c-b9ed-9c1a04247482}
std::string GenGuid()
{
	GUID stGuid;

	if (CoCreateGuid(&stGuid) != S_OK)
	{
		return "";
	}

	char buf[64]={0};
	sprintf(buf, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		stGuid.Data1,stGuid.Data2,stGuid.Data3,
		stGuid.Data4[0],stGuid.Data4[1],stGuid.Data4[2],stGuid.Data4[3],
		stGuid.Data4[4],stGuid.Data4[5],stGuid.Data4[6],stGuid.Data4[7]);
	return buf;
}*/


EntityMgr::EntityMgr():m_entity_id_seed(10000)
{
    m_pReflectionMgr = new ReflectionMgr;
}

Entity* EntityMgr::Create(const std::string& strClassName)
{
    // get unique Entity id
    YY_ENTITYID newid = m_entity_id_seed++;

    MetaClass* pec = m_pReflectionMgr->GetCreator(strClassName);
    throw_assert(pec!=NULL, "get creator from type:"<<strClassName<<" failed.");
    throw_assert(NULL != pec->create_func, "cannot create this virtual class:"<<strClassName);

    Entity* pEntity = (Entity*)pec->create_func();
    throw_assert(NULL != pEntity, "create Entity failed.");
    pEntity->SetEntMgr(this);
    pEntity->SetID(newid);
    pEntity->SetCreator(pec);

    m_pEntities[newid] = pEntity;

    pEntity->OnCreate();
    return pEntity;
}



void EntityMgr::Destroy(YY_ENTITYID id)
{
    // erase from Entity map
    std::map<YY_ENTITYID, Entity*>::iterator itor = m_pEntities.find(id);
    if(itor==m_pEntities.end())
    {
        return;
    }

    Entity* pEntity = itor->second;
    pEntity->OnDestroy();
    m_pEntities.erase(itor);

    // destroy
    MetaClass* pec = pEntity->GetCreator();
    pec->destroy_func(pEntity);
}

bool EntityMgr::SerializeTo(YY::Entity* pEntity, YY::VarList& args)
{
    throw_assert(NULL!=pEntity, "null check.");
    MetaClass* pMetaClass = pEntity->GetCreator();
    throw_assert(NULL!=pMetaClass, "null check.");

//    m_pReflectionMgr->SerializeTo(pEntity, pMetaClass->name, args);
    return true;
}

bool EntityMgr::IsInstanceOf(YY::Entity* pEntity, const std::string& strClassName)
{
    throw_assert(NULL!=pEntity, "null check.");
    return m_pReflectionMgr->IsInstanceOf(pEntity->GetCreator(), strClassName);
}

Entity* EntityMgr::Find(YY_ENTITYID id)
{
    std::map<YY_ENTITYID, Entity*>::iterator itor = m_pEntities.find(id);
    if(itor==m_pEntities.end())
        return NULL;

    return itor->second;
}

Entity* EntityMgr::Get(YY_ENTITYID id)
{
    Entity* pEnt = Find(id);
    throw_assert(NULL!=pEnt, "null check.");
    return pEnt;
}

void EntityMgr::SetGlobal(const char* name, YY::Var val)
{
    m_globalVars[name] = val;
}

YY::Var EntityMgr::GetGlobal(const char* name)
{
    std::map<std::string, YY::Var>::iterator itor = m_globalVars.find(name);
    throw_assert(itor!=m_globalVars.end(), "cannot find global val:"<<name);

    return itor->second;
}


bool EntityMgr::FindGlobal(const char* name)
{
    std::map<std::string, YY::Var>::iterator itor = m_globalVars.find(name);
    if(itor!=m_globalVars.end())
        return true;

    return false;
}


void EntityMgr::AddExcute(Entity* pEntity)
{
    throw_assert(NULL!=pEntity, "null check.");
    m_pExcute.push_back(pEntity->GetID());
}

void EntityMgr::RemoveExcute(Entity* pEntity)
{
    throw_assert(NULL!=pEntity, "null check.");
    int nSize = (int)m_pExcute.size();
    for(int i=0; i<nSize; i++)
    {
        YY_ENTITYID id = m_pExcute[i];
        if(id==pEntity->GetID())
        {
            m_pExcute.erase(m_pExcute.begin()+i);
            return;
        }
    }}


void EntityMgr::OnExcute(float sec)
{
    int nSize = (int)m_pExcute.size();
    for(int i=0; i<nSize; i++)
    {
        YY_ENTITYID id = m_pExcute[i];
        Entity* pEnt = Find(id);
        if(NULL==pEnt)
        {
            // have erase issure, then end this loop, begin next time.
            m_pExcute.erase(m_pExcute.begin()+i);
            break;
        }

        pEnt->OnExcute(sec);
    }
}


void EntityMgr::AddMsgProc(YY::Entity* pEntity, uint32 msg_id)
{
    m_input_mgr.AddMsgProc(pEntity, msg_id);
}

void EntityMgr::RemoveMsgProc(YY::Entity* pEntity, uint32 msg_id)
{
    m_input_mgr.RemoveMsgProc(pEntity, msg_id);
}

bool EntityMgr::OnMsg(uint32 msg_id, uint32 param1, uint32 param2)
{
    return m_input_mgr.Invoke(msg_id, param1, param2);
}


// 
// void EntityMgr::SetFieldVal(Entity* pEntity, const char* prop_name, const void* val)
// {
//     MetaClass* pMetaClass = pEntity->GetCreator();
//     std::map<std::string, MetaField*>::iterator itor_field = pMetaClass->fields.find(prop_name);
//     throw_assert(itor_field!=pMetaClass->fields.end(), "field must exist.");
// 
//     MetaField* pMetaField = itor_field->second;
//     pMetaField->Set(pEntity, val);
// }
// 
// void EntityMgr::GetFieldVal(Entity* pEntity, const char* prop_name, void* val)
// {
//     MetaClass* pMetaClass = pEntity->GetCreator();
//     std::map<std::string, MetaField*>::iterator itor_field = pMetaClass->fields.find(prop_name);
//     throw_assert(itor_field!=pMetaClass->fields.end(), "field must exist.");
// 
//     MetaField* pMetaField = itor_field->second;
//     pMetaField->field->Get(pEntity, val);
// }

NS_YY_END