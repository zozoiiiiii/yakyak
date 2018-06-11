/************************************************************************/
/* 
@author:    junliang
@brief:     cpp reflection

meta class is used in 
ORM(python : Object Relational Mapping)
ue4 blueprint
qt

@sample: https://github.com/zozoiiiiii/cppreflection
@time:      2018-1-8
*/
/************************************************************************/
#pragma once

#include "yy_meta_class.h"
#include "yy_meta_field.h"
#include "yy_meta_method.h"
#include "yy_meta_method_void.h"
#include <string>

#define YY_BEGIN(cls, parent_)                                     \
private:                                                           \
    static void* Create_##cls() { return new cls(); }              \
    static void Destroy_##cls(void* p) { delete ((cls*)p); }       \
public:                                                            \
    static MetaClass* MetaClassInstance()                          \
    {                                                              \
        static MetaClass s_MetaClass;                              \
		s_MetaClass.name = #cls;                                   \
        s_MetaClass.parent = #parent_;                             \
        s_MetaClass.create_func = &Create_##cls;                   \
        s_MetaClass.destroy_func = &Destroy_##cls;                 \
        MetaField* pField = NULL;                                  \
        MetaMethod* pMethod = NULL;

#define YY_END  return &s_MetaClass; }

#define YY_VIRTUAL_BEGIN(cls, parent_)                             \
public:                                                            \
    static MetaClass* MetaClassInstance()                          \
    {                                                              \
        static MetaClass s_MetaClass;                              \
        s_MetaClass.name = #cls;                                   \
        s_MetaClass.parent = #parent_;                             \
        MetaField* pField = NULL;                                  \
        MetaMethod* pMethod = NULL;


#define YY_FIELD(field_, name_, extension_)                        \
    pField = MakeField(field_);                                    \
    pField->desc = extension_;                                     \
	pField->name = name_;                                          \
    s_MetaClass.fields[pField->name] = pField;

#define YY_METHOD(method_, name_, extension_)                      \
    pMethod = MakeMethod(method_);                                 \
    pMethod->name = name_;                                         \
    pMethod->desc = extension_;                                    \
    s_MetaClass.methods[pMethod->name] = pMethod;
