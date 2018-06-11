/************************************************************************/
/* 
@author:    junliang
@brief:     
@time:      2017-8-7
*/
/************************************************************************/
#pragma once

#include "core/inc/yy_serializer.h"
#include "core/inc/yy_reflection_mgr.h"
#include "core/inc/yy_exception.h"
#include <vector>

NS_YY_BEGIN
class Serializer : public ISerializer
{
public:
    void SetReflectionMgr(IReflectionMgr* pReflectionMgr){m_pReflectionMgr = pReflectionMgr;}
    virtual bool SerializeTo(void* pClassInstance, const std::string& name, YY::VarList& args);
    virtual bool ParseFrom(void* pClassInstance, const std::string& name, const YY::VarList& args, int& read_index);


    //virtual int GetType(const MetaField* prop);
    virtual bool SetPropVal(void* pClassInstance, const MetaField* prop, const YY::Var& val);
    virtual void GetPropVal(void* pClassInstance, const MetaField* prop, YY::Var& var);
private:
    // ���һ�������Ǽ�¼��ǰ�Ѿ������ĵ������࣬��ֹ�ֳ��֣�������ѭ��
    bool InnerSerializeTo(void* pClassInstance, const std::string& name, YY::VarList& args, 
        std::vector<std::string>& serializedClasses);

    bool InnerParseFrom(void* pClassInstance, const std::string& name, const YY::VarList& args,
        int& nIndex, std::vector<std::string>& serializedClasses);

private:
    YY::IReflectionMgr* m_pReflectionMgr;
};
NS_YY_END