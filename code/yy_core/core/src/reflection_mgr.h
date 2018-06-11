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
    virtual MetaClass* GetCreator(const std::string& strClassName);
    virtual MetaClass* FindCreator(const std::string& strClassName);
    virtual void RegCreator(MetaClass* pCreator);

    virtual MetaField* FindMetaField(const std::string& strClassName, const std::string& strFieldName);
    virtual MetaField* GetMetaField(const std::string& strClassName, const std::string& strFieldName);

    virtual bool IsInstanceOf(MetaClass* pCreator, const std::string& strClassName);



    virtual void SetPropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName, const void* pPropVal);
    virtual const void* GetPropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName);
    virtual void SetBasePropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName, const YY::Var& val);
    virtual YY::Var GetBasePropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName);

    ISerializer* GetSerializer(){return m_pSerializer;}
private:

private:
    ISerializer* m_pSerializer;
    std::map<std::string, MetaClass*> m_creatorClasses;
};
NS_YY_END