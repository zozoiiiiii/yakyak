#include "reflection_mgr.h"
#include "core/inc/yy_exception.h"
#include <algorithm>
//#include <Windows.h>
#include "core/inc/yy_var.h"

NS_YY_BEGIN

ReflectionMgr::ReflectionMgr()
{}

MetaClass* ReflectionMgr::FindMetaClass(const std::string& strClassName)
{
    std::map<std::string, MetaClass*>::iterator itor = m_creatorClasses.find(strClassName);
    if (itor == m_creatorClasses.end())
        return NULL;

    MetaClass* pec = itor->second;
    return pec;
}


void ReflectionMgr::RegCreator(MetaClass* pCreator)
{
    std::map<std::string, MetaClass*>::iterator itor = m_creatorClasses.find(pCreator->name);
	if (itor != m_creatorClasses.end())
		return;

    m_creatorClasses[pCreator->name] = pCreator;
}


MetaField* ReflectionMgr::FindMetaField(const std::string& strClassName, const std::string& strFieldName)
{
    MetaClass* pMetaClass = FindMetaClass(strClassName);
    if(NULL == pMetaClass)
        return NULL;

    std::map<std::string, MetaField*>::iterator itor = pMetaClass->fields.find(strFieldName);
    if(itor == pMetaClass->fields.end())
        return NULL;

    return itor->second;
}

MetaField* ReflectionMgr::FindMetaFieldFromAll(const std::string& strClassName, const std::string& strFieldName)
{
	MetaClass* pMetaClass = FindMetaClass(strClassName);
	if (NULL == pMetaClass)
		return NULL;


	std::map<std::string, MetaField*>::iterator itor = pMetaClass->fields.find(strFieldName);
	if (itor == pMetaClass->fields.end())
	{
		return FindMetaFieldFromAll(pMetaClass->parent, strFieldName);
	}

	return itor->second;
}

void ReflectionMgr::GetAllMetaField(const std::string& strClassName, std::vector<MetaField*>& fields)
{
	MetaClass* pMetaClass = FindMetaClass(strClassName);
	if (NULL == pMetaClass)
		return;

	GetAllMetaField(pMetaClass->parent, fields);

	std::map<std::string, MetaField*>::iterator itor = pMetaClass->fields.begin();
	for (; itor != pMetaClass->fields.end(); itor++)
	{
		fields.push_back(itor->second);
	}
}

bool ReflectionMgr::IsInstanceOf(MetaClass* pCreator, const std::string& strClassName)
{
    throw_assert(NULL!=pCreator, "null check.");
    if(strClassName==pCreator->name)
        return true;

	// end of loop
    const std::string& strParent = pCreator->parent;
    if(strParent=="BaseObject" && strParent != strClassName)
        return false;

    MetaClass* pCreator_Parent = FindMetaClass(strParent);
    if(NULL==pCreator_Parent)
    {
        return false;
    }

    return IsInstanceOf(pCreator_Parent, strClassName);
}

void ReflectionMgr::SetBaseFieldVal(void* pClassInstance, MetaField* pMetaField, const YY::Var& val)
{
    int nType = pMetaField->var_type;
    switch(nType)
    {
    case YVT_BOOL:
        {
            bool tmpVal = val.GetBool();
            pMetaField->Set(pClassInstance, &tmpVal);
        }
        break;
    case YVT_SINT32:
        {
            int tmpVal = val.GetInt();
            pMetaField->Set(pClassInstance, &tmpVal);
        }
        break;
    case YVT_SINT64:
        {
            sint64 tmpVal = val.GetInt64();
            pMetaField->Set(pClassInstance, &tmpVal);
        }
        break;
    case YVT_FLOAT:
        {
            float tmpVal = val.GetFloat();
            pMetaField->Set(pClassInstance, &tmpVal);
        }
        break;
    case YVT_DOUBLE:
        {
            double tmpVal = val.GetDouble();
            pMetaField->Set(pClassInstance, &tmpVal);
        }
        break;
    case YVT_STRING:
        {
            std::string tmpVal = val.GetStr();
            pMetaField->Set(pClassInstance, &tmpVal);
        }
        break;
    case YVT_WSTRING:
        {
            std::wstring tmpVal = val.GetWstr();
            pMetaField->Set(pClassInstance, &tmpVal);
        }
        break;
    default:
        throw_assert(false, "should not run here.");
        break;
    }
}

YY::Var ReflectionMgr::GetBaseFieldVal(void* pClassInstance, MetaField* pMetaField)
{
    void* pPropVal = pMetaField->Get(pClassInstance);
    int nType = pMetaField->var_type;
    switch(nType)
    {
    case YVT_BOOL:
        {
            return *(bool*)pPropVal;
        }
        break;
    case YVT_SINT32:
        {
            return *(int*)pPropVal;
        }
        break;
    case YVT_SINT64:
        {
            return *(sint64*)pPropVal;
        }
        break;
    case YVT_FLOAT:
        {
            return *(float*)pPropVal;
        }
        break;
    case YVT_DOUBLE:
        {
            return *(double*)pPropVal;
        }
        break;
    case YVT_STRING:
        {
            return *(std::string*)pPropVal;
        }
        break;
    case YVT_WSTRING:
        {
            return *(std::wstring*)pPropVal;
        }
        break;
    default:
        throw_assert(false, "should not run here.");
        break;
    }

    return YY::Var();
}

NS_YY_END