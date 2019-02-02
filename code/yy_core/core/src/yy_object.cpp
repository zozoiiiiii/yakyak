#include "core/inc/yy_object.h"
#include "core/inc/yy_objectmgr.h"



NS_YY_BEGIN

bool BaseObject::IsInstanceOf(const std::string& strClassName)
{
	IReflectionMgr* pReflectionMgr = GetMgr()->GetReflectionMgr();
	return pReflectionMgr->IsInstanceOf(GetMetaClass(), strClassName);
}


IReflectionMgr* BaseObject::GetReflectionMgr()
{
	return GetMgr()->GetReflectionMgr();
}

IEventMgr* BaseObject::GetEventMgr()
{
	return GetMgr()->GetEventMgr();
}

bool BaseObject::SetFieldVal(const char* prop_name, const void* val)
{
	MetaField* pMetaField = GetMetaClass()->FindMetaField(prop_name);
	if (!pMetaField)
		return false;

	pMetaField->Set(this, val);
	return true;
}

bool BaseObject::GetFieldVal(const char* prop_name, void* val)
{
	MetaField* pMetaField = GetMetaClass()->FindMetaField(prop_name);
	if (!pMetaField)
		return false;

	val = pMetaField->Get(this);
	return true;
}

void BaseObject::Invoke(void* result, const char* name, void* parameters[])
{
	throw_assert(NULL != m_pMetaClass, "null check.");
	std::map<std::string, MetaMethod*>::iterator itor = m_pMetaClass->methods.find(name);
	throw_assert(itor != m_pMetaClass->methods.end(), "not exist method:" << name);

	MetaMethod* pMethodCreator = m_pMetaClass->methods[name];
	pMethodCreator->Invoke(result, this, parameters);
}


void BaseObject::Invoke(void* result, const char* name)
{
	void* parameters[1];
	Invoke(result, name, parameters);
}

void BaseObject::Invoke(void* result, const char* name, void* p1)
{
	void* parameters[1];
	parameters[0] = p1;
	Invoke(result, name, parameters);
}

void BaseObject::Invoke(void* result, const char* name, void* p1, void* p2)
{
	void* parameters[2];
	parameters[0] = p1;
	parameters[1] = p2;
	Invoke(result, name, parameters);
}

void BaseObject::Invoke(void* result, const char* name, void* p1, void* p2, void* p3)
{
	void* parameters[3];
	parameters[0] = p1;
	parameters[1] = p2;
	parameters[2] = p3;
	Invoke(result, name, parameters);
}


void BaseObject::Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4)
{
	void* parameters[4];
	parameters[0] = p1;
	parameters[1] = p2;
	parameters[2] = p3;
	parameters[3] = p4;
	Invoke(result, name, parameters);
}

void BaseObject::Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4, void* p5)
{
	void* parameters[5];
	parameters[0] = p1;
	parameters[1] = p2;
	parameters[2] = p3;
	parameters[3] = p4;
	parameters[4] = p5;
	Invoke(result, name, parameters);
}
NS_YY_END


