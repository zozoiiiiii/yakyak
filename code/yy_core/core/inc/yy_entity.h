/************************************************************************/
/*
@author:    junliang
@brief:     entity
1. create/destroy
2. parent/child
3. meta class

http://gad.qq.com/article/detail/96
// rtti: http://flipcode.com/archives/Simple_RTTI_For_C.shtml
@time:      20160120
*/
/************************************************************************/
#pragma once

#include <string>
#include <map>
#include <vector>
#include "reflection/yy_type.h"
#include "reflection/yy_reflection.h"
#include "core/inc/yy_var.h"
#include "core/inc/yy_varlist.h"
#include "core/inc/yy_exception.h"

// guid
#ifndef YY_ENTITYID
#define  YY_ENTITYID sint64
#define YY_INVALID_ENTITYID 0
#endif

NS_YY_BEGIN

class IEntityMgr;
class Entity
{
public:
    Entity():m_pEntityMgr(NULL), m_parent(YY_INVALID_ENTITYID){}
    virtual ~Entity(){}
    // oncreate without parameters, to help the serializer.
    virtual void OnCreate(){}
    virtual void OnDestroy(){}
    virtual void OnExcute(float sec){}
    virtual bool SerializeTo(YY::VarList& args);
    virtual bool ParseFrom(const YY::VarList& args, int& read_index);

    IEntityMgr* GetMgr(){return m_pEntityMgr;}
    YY_ENTITYID GetID(){return m_id;}
    MetaClass* GetCreator(){return m_pMetaClass;};

    void AddChild(YY_ENTITYID id);
    void RemoveChild(YY_ENTITYID id);
    bool IsChildExist(YY_ENTITYID id);
    YY::VarList GetChildList();
    void SetParent(YY_ENTITYID id);
    YY_ENTITYID GetParent();

    // custom prop: SetCustomProp("test", 11); SetCustomProp("test1", true);
    void SetCustomProp(const std::string& name, const YY::Var& val);
    int GetCustomPropInt(const std::string& name);
    float GetCustomPropFloat(const std::string& name);
    std::string GetCustomPropString(const std::string& name);

    // reflection
//     int GetPropInt(const std::string& name);
//     void SetPropInt(const std::string& name, int val);
//     float GetPropFloat(const std::string& name);
//     void SetPropFloat(const std::string& name, float val);
//     std::string GetPropString(const std::string& name);
//     void SetPropString(const std::string& name, const std::string& val);

    void Invoke(void* result, const char* name);

    // varlist not support class, so use void* better.
    //void Invoke(void* result, const char* name, const YY::VarList& args);
    void Invoke(void* result, const char* name, void* p1);
    void Invoke(void* result, const char* name, void* p1, void* p2);
    void Invoke(void* result, const char* name, void* p1, void* p2, void* p3);
	void Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4);
	void Invoke(void* result, const char* name, void* p1, void* p2, void* p3, void* p4, void* p5);


    // put into input module in future.
    // use default msg proc.
    virtual bool OnMsg(uint32 msg_id, uint32 p1, uint32 p2) { return false; }
private:
    void SetEntMgr(IEntityMgr* pMgr){m_pEntityMgr=pMgr;}
    void SetID(YY_ENTITYID id){m_id = id;}
    void SetCreator(MetaClass* creator){ m_pMetaClass = creator;}

    void Invoke(void* result, const char* name, void* parameters[]);
    bool IsCustomPropExist(const std::string& name);
protected:
    std::vector<YY_ENTITYID> m_children;
private:
    YY_ENTITYID m_id;
    MetaClass* m_pMetaClass;
    IEntityMgr* m_pEntityMgr;
    YY_ENTITYID m_parent;
    friend class EntityMgr;
    std::map<std::string, YY::Var> m_custom_props;
};
NS_YY_END


