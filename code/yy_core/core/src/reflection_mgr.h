/************************************************************************/
/*
@author:    junliang
@brief:     reflection manager
@time:      20160120
*/
/************************************************************************/
#pragma once

#include "../inc/yy_reflection_mgr.h"
#include "../inc/yy_serializer.h"
NS_YY_BEGIN

class ReflectionMgr : public YY::IReflectionMgr
{
public:
    ReflectionMgr();
    virtual ~ReflectionMgr(){}
    virtual MetaClass* FindMetaClass(const std::string& strClassName);
    virtual void RegCreator(MetaClass* pCreator);

	virtual MetaField* FindMetaField(const std::string& strClassName, const std::string& strFieldName);
	virtual MetaField* FindMetaFieldFromAll(const std::string& strClassName, const std::string& strFieldName);
	virtual void GetAllMetaField(const std::string& strClassName, std::vector<MetaField*>& fields);

    virtual bool IsInstanceOf(MetaClass* pCreator, const std::string& strClassName);

	virtual void SetBaseFieldVal(void* pClassInstance, MetaField* pMetaField, const YY::Var& val);
	virtual YY::Var GetBaseFieldVal(void* pClassInstance, MetaField* pMetaField);


private:
    std::map<std::string, MetaClass*> m_creatorClasses;
};
NS_YY_END