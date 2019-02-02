/************************************************************************/
/* 
@author:    junliang
@brief:     

game obj + transform
--------------------comp1
--------------------comp2

@time:      11/23/2016
*/
/************************************************************************/
#pragma once
//#include "core/inc/yy_macros.h"
#include "core/inc/yy_object.h"
#include "core/inc/yy_ObjectMgr.h"
#include "math/inc/yy_vec3f.h"
#include <string>
#include "transform.h"
#include "yy_world/inc/i_game_obj.h"


class GameObj : public IGameObj
{
    YY_BEGIN(GameObj, "IGameObj");
    YY_FIELD(&GameObj::m_name, "name", "");
	YY_FIELD(&GameObj::m_transform, "transform", "");
    YY_END
public:
    GameObj();
    virtual ~GameObj(){}
    virtual void OnCreate();
    virtual void OnDestroy(){}
    virtual std::string GetName(){return m_name;}
    virtual void SetName(const std::string& name){m_name = name;}
    virtual IRender* GetRender();
    virtual ITransform* GetTransform(){return &m_transform;}
    virtual void GetComps(std::vector<IComponent*>& comps){comps=m_comps;}
    virtual IComponent* FindComp(const std::string& name);
    virtual IComponent* GetComp(const std::string& name);
    virtual IComponent* AddComp(const std::string& name);

    virtual void OnUpdate(float sec);
    virtual void OnAddRender(IBatchGroup* pBatchGroup);
    virtual void OnRender(RenderContext* pCxt);

    virtual void SetParent(IGameObj* pObj);
    virtual IGameObj* FindParent();
    virtual void GetChildren(std::vector<IGameObj*>& objs);
    virtual void AddChild(IGameObj* pObj);

    // shadow
    virtual void SetCastShadow(bool flag){m_cast_shadow = flag;}
    virtual bool GetCastShadow(){return m_cast_shadow;}
    virtual void SetReceiveShadow(bool flag){m_receive_shadow = flag;}
    virtual bool GetReceiveShader(){return m_receive_shadow;}

    // water reflect.

    virtual bool SerializeTo(YY::VarList& args);
    virtual bool ParseFrom(const YY::VarList& args, int& read_index);

    virtual YY::AABB GetAABB();
    virtual YY::AABB GetAABB_World();
    virtual void SetAABB(const YY::AABB &aabb);
    virtual bool GetVisible(){return m_bVisible;}
    virtual void SetVisible(bool visible){m_bVisible = visible;}
protected:
    YY::Vec3f m_test;
    bool m_bool_test;

//    IScene* m_pScene;
    Transform m_transform;
    std::vector<IComponent*> m_comps;
    YY::AABB m_aabb;

    IGameObj* m_pParent;
    std::vector<IGameObj*> m_childrens;

    std::string m_name;
    bool m_receive_shadow;
    bool m_cast_shadow;
    bool m_bVisible;        // 是否显示(通过摄像机来裁剪判断)
};
