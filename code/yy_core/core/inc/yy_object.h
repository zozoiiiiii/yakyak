/************************************************************************/
/*
@author:    junliang
@brief:     base object
	1. create/destroy
	2. meta class
@time:      20160120
*/
/************************************************************************/
#pragma once

#include <string>
#include <map>
#include <vector>
#include <errno.h>

#include "reflection/yy_type.h"
#include "reflection/yy_reflection.h"
#include "core/inc/yy_var.h"
#include "core/inc/yy_varlist.h"
#include "core/inc/yy_exception.h"
#include "core/inc/yy_file.h"
#include "core/inc/yy_reflection_mgr.h"
#include "core/inc/yy_string.h"
#include "core/inc/yy_memory.h"


// guid
#ifndef YY_OBJECTID
#define  YY_OBJECTID sint64
#define YY_INVALID_OBJECTID 0
#endif

typedef std::map<std::string, YY::Var> VariantMap;

NS_YY_BEGIN
class IEventMgr;
class IObjectMgr;
class IReflectionMgr;
class BaseObject
{
public:
	virtual void OnCreate(const VariantMap& args) {}
    virtual void OnDestroy(){}
	virtual void OnExcute(float sec) {}
	virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2) { return false; }

    IObjectMgr* GetMgr(){return m_pObjectMgr;}
	IReflectionMgr* GetReflectionMgr();
    YY_OBJECTID GetID(){return m_id;}
	MetaClass* GetMetaClass() { return m_pMetaClass; };
	bool IsInstanceOf(const std::string& strClassName);
	IEventMgr* GetEventMgr();

	bool SetFieldVal(const char* prop_name, const void* val);
	bool GetFieldVal(const char* prop_name, void* val);

    void Invoke(void* result, const char* name);
    void Invoke(void* result, const char* name, void* p1);
    void Invoke(void* result, const char* name, void* p1, void* p2);
    void Invoke(void* result, const char* name, void* p1, void* p2, void* p3);
	void Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4);
	void Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4, void* p5);

private:
    void SetEntMgr(IObjectMgr* pMgr){m_pObjectMgr=pMgr;}
    void SetID(YY_OBJECTID id){m_id = id;}
    void SetCreator(MetaClass* creator){ m_pMetaClass = creator;}
    void Invoke(void* result, const char* name, void* parameters[]);
private:
    YY_OBJECTID m_id;
    MetaClass* m_pMetaClass;
    IObjectMgr* m_pObjectMgr;
    friend class ObjectMgr;
};


NS_YY_END


