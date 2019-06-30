#include "core/inc/yy_object.h"
#include "core/inc/yy_objectmgr.h"



NS_YY_BEGIN

BaseObject* BaseObject::parseFromFile(IObjectMgr* pObjMgr, const std::string& file, YY_OBJECTID id)
{
	FILE* pFile = fopen(file.c_str(), "r");
	if (nullptr == pFile)
		return nullptr;

	fseek(pFile, 0, SEEK_END);
	int nSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	YY::AutoMem<char, 512> buf(nSize);
	fread(buf.GetBuf(), nSize, 1, pFile);
	fclose(pFile);

	return parseFrom(pObjMgr, buf.GetBuf(), id);
}

void BaseObject::parseFromObjectString(const std::string& data)
{
	Document doc;
	doc.Parse(data.c_str());
	if (!doc.IsObject())
		return;

	parseFromObject(&doc);
}

bool BaseObject::parseFromObject(const rapidjson::Value* pObject)
{
	IReflectionMgr* pReflectionMgr = GetReflectionMgr();
	return parseFromProperties(pObject, GetMetaClass(), this);
}

bool BaseObject::parseFromProperties(const rapidjson::Value* pObject, MetaClass* pMetaClass, void* pInstance)
{
	Value::ConstMemberIterator propertiesItr = pObject->FindMember("properties");
	if (propertiesItr == pObject->MemberEnd())
		return true;

	const Value* properties = &propertiesItr->value;

	bool bRet = true;
	IReflectionMgr* pReflectionMgr = GetReflectionMgr();
	YY::Var var;
	for (Value::ConstMemberIterator itr = properties->MemberBegin(); itr != properties->MemberEnd(); ++itr)
	{
		std::string key = itr->name.GetString();
		MetaField* pMetaField = pReflectionMgr->FindMetaFieldFromAll(pMetaClass->name, key.c_str());
		if (!pMetaField)
		{
			bRet = false;
			continue;
		}

		if (!itr->value.IsObject())
		{
			// base variant
			std::string strValue = itr->value.GetString();
			var.SetType(pMetaField->var_type);
			var.ParseFrom(strValue);
			pReflectionMgr->SetBaseFieldVal(pInstance, pMetaField, var);
		}
		else
		{
			// object
			if (pMetaField->var_type != YVT_CLASS)
			{
				bRet = false;
				continue;
			}

			MetaClass* pSubMetaClass = pReflectionMgr->FindMetaClass(pMetaField->type_name);
			if (!pSubMetaClass)
			{
				bRet = false;
				continue;
			}

			// property object should not baseobject
			if (pReflectionMgr->IsInstanceOf(pSubMetaClass, "BaseObject"))
			{
				bRet = false;
				continue;
			}

			void* pSubObject = pMetaField->Get(pInstance);
			parseFromProperties(&itr->value, pSubMetaClass, pSubObject);
		}
	}

	return bRet;
}

BaseObject* BaseObject::parseFrom(IObjectMgr* pObjMgr, const std::string& data, YY_OBJECTID id)
{
	IReflectionMgr* pReflectionMgr = pObjMgr->GetReflectionMgr();
	YY::Var var;
	Document doc;
	doc.Parse(data.c_str());
	if (!doc.IsObject())
		return nullptr;

	// must 1 member: class name
	if (doc.MemberCount() != 1)
		return nullptr;

	Value::ConstMemberIterator classItr = doc.MemberBegin();
	if (classItr == doc.MemberEnd())
		return nullptr;

	std::string strClass = classItr->name.GetString();
	MetaClass* pMetaClass = pReflectionMgr->FindMetaClass(strClass.c_str());
	if (!pMetaClass)
		return nullptr;

	// create baseobject from meta name
	BaseObject* pObject = pObjMgr->Create(strClass, id);
	if (!pObject)
		return nullptr;

	if (!pObject->parseFromObject(&classItr->value))
		return nullptr;

	return pObject;
}


bool BaseObject::serializeProperties(rapidjson::Document* doc, rapidjson::Value* pObject,MetaClass* pMetaClass, void* pInstance)
{
	rapidjson::Value properties;
	properties.SetObject();

	IReflectionMgr* pReflectionMgr = GetReflectionMgr();
	std::vector<MetaField*> fields;
	pReflectionMgr->GetAllMetaField(pMetaClass->name, fields);
	for (int i = 0; i < fields.size(); i++)
	{
		MetaField* pMetaField = fields[i];
		std::string key = pMetaField->name;
		rapidjson::Value rjKey(key.c_str(), doc->GetAllocator());

		if (pMetaField->var_type == YVT_CLASS)
		{
 			std::string strChildClassName = pMetaField->type_name;
 			void* pChildInstance = pMetaField->Get(pInstance);
 			MetaClass* pChildMetaClass = pReflectionMgr->FindMetaClass(strChildClassName);
 			if(!pChildMetaClass)
 				continue;

			// this object should not hierarchy of BaseObject(consider as child), like vec3f
			if (pReflectionMgr->IsInstanceOf(pChildMetaClass, "BaseObject"))
				continue;

			rapidjson::Value rjVal;
			rjVal.SetObject();
			serializeProperties(doc, &rjVal, pChildMetaClass, pChildInstance);
			properties.AddMember(rjKey, rjVal, doc->GetAllocator());
		}
		else
		{
			// property is base type
			YY::Var value = pReflectionMgr->GetBaseFieldVal(pInstance, pMetaField);
			std::string strFieldVal = value.SerializeTo();
			rapidjson::Value rjVal(strFieldVal.c_str(), doc->GetAllocator());
			properties.AddMember(rjKey, rjVal, doc->GetAllocator());
		}
	}

	pObject->AddMember("properties", properties, doc->GetAllocator());
	return true;
}

bool BaseObject::serializeObjectTo(rapidjson::Document* doc, rapidjson::Value* pObject)
{
	bool bResult = true;
	IReflectionMgr* pReflectionMgr = GetReflectionMgr();

	// serialize properties data
	if (!serializeProperties(doc, pObject, GetMetaClass(), this))
		return false;

	return true;
}



std::string BaseObject::serializeTo()
{
	rapidjson::Document doc;
	doc.SetObject();

	rapidjson::Value objectVal;
	objectVal.SetObject();
	if (!serializeObjectTo(&doc, &objectVal))
		return false;

	rapidjson::Value key(GetMetaClass()->name.c_str(), doc.GetAllocator());
	doc.AddMember(key, objectVal, doc.GetAllocator());


	// convert rapidjson into string
	rapidjson::StringBuffer strBuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strBuf);
	doc.Accept(writer);
	return strBuf.GetString();
}

bool BaseObject::serializeToFile(const std::string& file)
{
	FILE* pFile = fopen(file.c_str(), "w");
	if (!pFile)
		return false;

	std::string str = serializeTo();
	fwrite(str.c_str(), str.length(), 1, pFile);
	fclose(pFile);
	return true;
}




/////////////////////////////////////////////////
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


