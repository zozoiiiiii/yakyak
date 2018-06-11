/************************************************************************/
/* 
@author:    junliang
@brief:     
 field not change no need to save, so every field can set default value when YY_FIELD,
but how to init, like struct Point, so have no idea, still serialize all fields.



@time:      2017-6-15
*/
/************************************************************************/
#pragma once
#include "reflection/yy_type.h"
#include "reflection/yy_reflection.h"
#include "core/inc/yy_varlist.h"
#include <string>

NS_YY_BEGIN
class ISerializer
{
public:
    virtual bool SerializeTo(void* pClassInstance, const std::string& name, YY::VarList& data)=0;
    virtual bool ParseFrom(void* pClassInstance, const std::string& name, const YY::VarList& data, int& read_index) = 0;


    virtual bool SetPropVal(void* pClassInstance, const MetaField* prop, const YY::Var& val) = 0;
    virtual void GetPropVal(void* pClassInstance, const MetaField* prop, YY::Var& var) = 0;
};

NS_YY_END