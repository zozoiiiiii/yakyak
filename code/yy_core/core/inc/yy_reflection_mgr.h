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
    virtual MetaClass* FindMetaClass(const std::string& strClassName) = 0;
    virtual void RegCreator(MetaClass* pCreator) = 0;
	virtual void GetAllMetaField(const std::string& strClassName, std::vector<MetaField*>& fields) = 0;
    virtual MetaField* FindMetaField(const std::string& strClassName, const std::string& strFieldName) = 0;
	virtual MetaField* FindMetaFieldFromAll(const std::string& strClassName, const std::string& strFieldName) = 0;
    virtual bool IsInstanceOf(MetaClass* pCreator, const std::string& strClassName)=0;

	// field type is not class, then can store data in var
	virtual void SetBaseFieldVal(void* pClassInstance, MetaField* pMetaField, const YY::Var& val) = 0;
	virtual YY::Var GetBaseFieldVal(void* pClassInstance, MetaField* pMetaField) = 0;
};
NS_YY_END
