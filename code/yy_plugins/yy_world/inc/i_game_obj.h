/************************************************************************/
/* 
@author:    junliang
@brief:     game obj + component
@time:      11/23/2016
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include "core/inc/yy_object.h"
#include "core/inc/yy_ObjectMgr.h"
#include "math/inc/yy_vec3f.h"
#include <string>
#include "i_transform.h"
#include "math/inc/yy_aabb.h"
#include "yy_render/inc/i_render.h"

class IScene;
class IRender;
class IComponent;
class IGameObj : public IBatch
{
    YY_VIRTUAL_BEGIN(IGameObj, "BaseObject");
    YY_END
public:
    virtual std::string GetName() = 0;
    virtual void SetName(const std::string& name) = 0;
    virtual ITransform* GetTransform()=0;
    virtual IRender* GetRender() = 0;
    virtual IScene* GetScene(){return m_pScene;}
    virtual void GetComps(std::vector<IComponent*>& comps)=0;
    virtual IComponent* FindComp(const std::string& name)=0;
    virtual IComponent* GetComp(const std::string& name)=0;
    virtual IComponent* AddComp(const std::string& name)=0;

    virtual void OnUpdate(float sec)=0;
    virtual void OnAddRender(IBatchGroup* pBatchGroup)=0;
    virtual void OnRender(RenderContext* pCxt)=0;

    virtual void SetParent(IGameObj* pObj) = 0;
    virtual IGameObj* FindParent() = 0;
    virtual void GetChildren(std::vector<IGameObj*>& objs) = 0;
    virtual void AddChild(IGameObj* pObj) = 0;

    virtual void SetCastShadow(bool flag) = 0;
    virtual bool GetCastShadow() = 0;
    virtual void SetReceiveShadow(bool flag) = 0;
    virtual bool GetReceiveShader() = 0;

    virtual YY::AABB GetAABB() = 0;
    virtual YY::AABB GetAABB_World() = 0;
    virtual void SetAABB(const YY::AABB &aabb) = 0;
    virtual bool GetVisible() = 0;
    virtual void SetVisible(bool visible) = 0;

private:
    void SetScene(IScene* pScene){m_pScene=pScene;}
private:
    friend class Scene;
    IScene* m_pScene;
};

class IComponent : public IBatch
{
public:
    IGameObj* GetOwner(){return m_pOwner;}
    IRender* GetRender(){return GetOwner()->GetRender();}
    ITransform* GetTransform(){return GetOwner()->GetTransform();}
    virtual void OnUpdate(float sec){}
    virtual void OnAddRender(IBatchGroup* pBatchGroup){pBatchGroup->AddSolidBatch(this);}
    virtual void OnRender(RenderContext* pCxt){}
private:
    void SetOwner(IGameObj* pOwner){m_pOwner = pOwner;}
private:
    IGameObj* m_pOwner;
    friend class GameObj;
};
