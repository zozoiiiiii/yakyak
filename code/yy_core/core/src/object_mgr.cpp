#include "object_mgr.h"
#include <algorithm>
#include "core/inc/yy_var.h"
#include "core/inc/yy_varlist.h"
#include "core/inc/yy_exception.h"
#include "reflection_mgr.h"
#include "event_mgr.h"


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


ObjectMgr::ObjectMgr():m_BaseObject_id_seed(10000)
{
    m_pReflectionMgr = new ReflectionMgr;
	m_pEventMgr = new EventMgr;
	m_pInputMgr = new InputMgr;
}

BaseObject* ObjectMgr::BeginCreate(const std::string& strClassName)
{
    // get unique BaseObject id
    YY_OBJECTID newid = m_BaseObject_id_seed++;

    MetaClass* pec = m_pReflectionMgr->FindMetaClass(strClassName);
    throw_assert(pec!=NULL, "get creator from type:"<<strClassName<<" failed.");
    throw_assert(NULL != pec->create_func, "cannot create this virtual class:"<<strClassName);

    BaseObject* pBaseObject = (BaseObject*)pec->create_func();
    throw_assert(NULL != pBaseObject, "create BaseObject failed.");
    pBaseObject->SetEntMgr(this);
    pBaseObject->SetID(newid);
    pBaseObject->SetCreator(pec);

    m_pEntities[newid] = pBaseObject;

    return pBaseObject;
}

void ObjectMgr::EndCreate(BaseObject* pObject, const VariantMap& args)
{
	if (nullptr == pObject)
		return;

	pObject->OnCreate(args);
	m_pEventMgr->Invoke(pObject->GetID(), "ObjectCreated", pObject->GetID());
}


BaseObject* ObjectMgr::Create(const std::string& strClassName, const VariantMap& args)
{
	YY::BaseObject* pObject = BeginCreate(strClassName);
	EndCreate(pObject, args);
	return pObject;
}


void ObjectMgr::Destroy(YY_OBJECTID id)
{
    // erase from BaseObject map
    std::map<YY_OBJECTID, BaseObject*>::iterator itor = m_pEntities.find(id);
    if(itor==m_pEntities.end())
    {
        return;
    }

    BaseObject* pBaseObject = itor->second;
    pBaseObject->OnDestroy();
	m_pEventMgr->Invoke(pBaseObject->GetID(), "ObjectDestroyed", pBaseObject->GetID());
    m_pEntities.erase(itor);

    // destroy
    MetaClass* pec = pBaseObject->GetMetaClass();
    pec->destroy_func(pBaseObject);
}


YY::Log* ObjectMgr::GetLog()
{
	static YY::Log s_log;
	return &s_log;
}

BaseObject* ObjectMgr::Find(YY_OBJECTID id)
{
    std::map<YY_OBJECTID, BaseObject*>::iterator itor = m_pEntities.find(id);
    if(itor==m_pEntities.end())
        return NULL;

    return itor->second;
}

BaseObject* ObjectMgr::Get(YY_OBJECTID id)
{
    BaseObject* pEnt = Find(id);
    throw_assert(NULL!=pEnt, "null check.");
    return pEnt;
}

void ObjectMgr::SetGlobal(const char* name, YY::Var val)
{
    m_globalVars[name] = val;
}

YY::Var ObjectMgr::FindGlobal(const char* name)
{
    std::map<std::string, YY::Var>::iterator itor = m_globalVars.find(name);
    if(itor!=m_globalVars.end())
        return itor->second;

	return YY::Var();
}


BaseObject* ObjectMgr::FindGlobalObject(const char* name)
{
	const YY::Var& value = FindGlobal(name);
	if (value.GetType() != YVT_SINT64)
		return nullptr;

	YY_OBJECTID id = value.GetInt64();
	return Find(id);
}

void ObjectMgr::AddExcute(BaseObject* pBaseObject)
{
    throw_assert(NULL!=pBaseObject, "null check.");
    m_pExcute.push_back(pBaseObject->GetID());
}

void ObjectMgr::RemoveExcute(BaseObject* pBaseObject)
{
    throw_assert(NULL!=pBaseObject, "null check.");
    int nSize = (int)m_pExcute.size();
    for(int i=0; i<nSize; i++)
    {
        YY_OBJECTID id = m_pExcute[i];
        if(id==pBaseObject->GetID())
        {
            m_pExcute.erase(m_pExcute.begin()+i);
            return;
        }
    }}


void ObjectMgr::OnExcute(float sec)
{
    int nSize = (int)m_pExcute.size();
    for(int i=0; i<nSize; i++)
    {
        YY_OBJECTID id = m_pExcute[i];
        BaseObject* pEnt = Find(id);
        if(NULL==pEnt)
        {
            // have erase issure, then end this loop, begin next time.
            m_pExcute.erase(m_pExcute.begin()+i);
            break;
        }

        pEnt->OnExcute(sec);
    }
}


// void ObjectMgr::AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id)
// {
//     m_input_mgr.AddMsgProc(pBaseObject, msg_id);
// }
// 
// void ObjectMgr::RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id)
// {
//     m_input_mgr.RemoveMsgProc(pBaseObject, msg_id);
// }
// 
// bool ObjectMgr::OnMsg(uint32 msg_id, uint32 param1, uint32 param2)
// {
//     return m_input_mgr.Invoke(msg_id, param1, param2);
// }


// 
// void ObjectMgr::SetFieldVal(BaseObject* pBaseObject, const char* prop_name, const void* val)
// {
//     MetaClass* pMetaClass = pBaseObject->FindMetaClass();
//     std::map<std::string, MetaField*>::iterator itor_field = pMetaClass->fields.find(prop_name);
//     throw_assert(itor_field!=pMetaClass->fields.end(), "field must exist.");
// 
//     MetaField* pMetaField = itor_field->second;
//     pMetaField->Set(pBaseObject, val);
// }
// 
// void ObjectMgr::GetFieldVal(BaseObject* pBaseObject, const char* prop_name, void* val)
// {
//     MetaClass* pMetaClass = pBaseObject->FindMetaClass();
//     std::map<std::string, MetaField*>::iterator itor_field = pMetaClass->fields.find(prop_name);
//     throw_assert(itor_field!=pMetaClass->fields.end(), "field must exist.");
// 
//     MetaField* pMetaField = itor_field->second;
//     pMetaField->field->Get(pBaseObject, val);
// }


bool ObjectMgr::OnMsg(uint32 msg_id, uint32 param1, uint32 param2)
{
	return m_pInputMgr->Invoke(msg_id, param1, param2);
}

void ObjectMgr::AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id)
{
	m_pInputMgr->AddMsgProc(pBaseObject, msg_id);
}

void ObjectMgr::RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id)
{
	m_pInputMgr->RemoveMsgProc(pBaseObject, msg_id);
}
NS_YY_END