/************************************************************************/
/*
@author:    junliang
@brief:     reflection manager
@time:      20160120
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include "reflection/yy_reflection.h"
#include "core/inc/yy_varlist.h"
#include "core/inc/yy_var.h"
#include "yy_serializer.h"
#include <string>

NS_YY_BEGIN
class IReflectionMgr
{
public:
    virtual ~IReflectionMgr(){}
    virtual MetaClass* GetCreator(const std::string& strClassName) = 0;
    virtual MetaClass* FindCreator(const std::string& strClassName) = 0;
    virtual void RegCreator(MetaClass* pCreator) = 0;
    virtual MetaField* FindMetaField(const std::string& strClassName, const std::string& strFieldName) = 0;
    virtual MetaField* GetMetaField(const std::string& strClassName, const std::string& strFieldName) = 0;

    virtual bool IsInstanceOf(MetaClass* pCreator, const std::string& strClassName)=0;

    // "int"->YVT_SINT32, "Vec2"->YVT_CLASS
    //virtual int GetType(const PropCreator* prop)=0;

    virtual void SetPropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName, const void* pPropVal) = 0;
    virtual const void* GetPropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName) = 0;
    virtual void SetBasePropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName, const YY::Var& val) = 0;
    virtual YY::Var GetBasePropVal(void* pClassInstance, const std::string& pClassName, const std::string& pPropName) = 0;

    //virtual void Invoke(void* result, void* obj, const char* name, void* parameters[])=0;

    virtual ISerializer* GetSerializer() = 0;
};
NS_YY_END
