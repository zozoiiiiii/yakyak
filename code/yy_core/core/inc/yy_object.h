/************************************************************************/
/*
@author:    junliang
@brief:     base object
	1. create/destroy
	2. meta class

	{
	"class": "Object1"
	"properties":{
		"x":"0",
		"y":"0",
		object1:{"properties":{x:10}},
		object2:{"properties":{y:10}}
		}
	}
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
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
typedef rapidxml::xml_document<char> xml_document;
typedef rapidxml::xml_node<char> xml_node;
typedef rapidxml::xml_attribute<char> xml_attribute;

#include "../external/rapidjson/document.h"
#include "../external/rapidjson/writer.h"
#include "../external/rapidjson/prettywriter.h"
#include "../external/rapidjson/stringbuffer.h"
using namespace rapidjson;

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
	YY_BEGIN(BaseObject, "");
	YY_END
public:
	static BaseObject* parseFrom(IObjectMgr* pObjMgr, const std::string& data, YY_OBJECTID id = YY_INVALID_OBJECTID);
	static BaseObject* parseFromFile(IObjectMgr* pObjMgr, const std::string& file, YY_OBJECTID id = YY_INVALID_OBJECTID);
	std::string serializeTo();
	bool serializeToFile(const std::string& file = "");

	virtual void OnCreate() {}
    virtual void OnDestroy(){}
	virtual void OnExcute(float sec) {}
	virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2) { return false; }
	virtual BaseObject* duplicate() { return nullptr; }
	virtual void parseFromObjectString(const std::string& data);

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

protected:
	virtual bool parseFromObject(const rapidjson::Value* value);	
	virtual bool serializeObjectTo(rapidjson::Document* doc, rapidjson::Value* value);

	// property object should not contain baseobject(can used as child), like vec3f
	virtual bool serializeToProperties(rapidjson::Document* doc, rapidjson::Value* value, MetaClass* pMetaClass, void* pInstance);
	virtual bool parseFromProperties(const rapidjson::Value* value, MetaClass* pMetaClass, void* pInstance);
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



//interface
#define LOG_ERR(Msg)          this->GetMgr()->GetLog()->LogData((int)Log::LL_ERROR, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
#define LOG_TRACE(Msg)        this->GetMgr()->GetLog()->LogData((int)Log::LL_TRACE, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
#define LOG_DEBUG(Msg)        this->GetMgr()->GetLog()->LogData((int)Log::LL_DEBUG, __FILE__, __LINE__, Log::StreamFormatter()<<Msg)
NS_YY_END


