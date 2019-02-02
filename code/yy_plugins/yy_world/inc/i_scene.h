/************************************************************************/
/* 
@author:    junliang
@brief:     scene
@time:      2017-6-9
*/
/************************************************************************/
#pragma once


#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"
#include "math/inc/yy_ray.h"

// RenderType
enum
{
    FORWARD_SHADING,        // 前向渲染
    DEFERRED_SHADING        // 延迟渲染，可以处理更多光源
};

class IWorld;
class IScene : public YY::BaseObject
{
public:
    virtual void Update(float sec) = 0;
    virtual void Render() = 0;

    virtual RenderContext* GetRenderCxt() = 0;
    virtual IRender* GetRender() = 0;

    virtual IGameObj* CreateObj(IGameObj* pParent=NULL) = 0;
    virtual bool AddObj(IGameObj* pObj) = 0;
    virtual bool RemoveObj(IGameObj* pObj) = 0;

    virtual void SetMainCamera(IGameObj* pCamera) = 0;
    virtual IGameObj* GetMainCamera() = 0;

    // get objs
    virtual IGameObj* FindObjByName(const std::string& name) = 0;

    virtual IGameObj* HitByRay(const YY::Ray &ray) = 0;

    // 可视对象
    virtual int GetVisibleListNum() = 0;
    virtual YY_OBJECTID GetVisibleObjByID(int index) = 0;

    // 所有对象
    virtual int GetObjListNum() = 0;
    virtual YY_OBJECTID GetObjByID(int index) = 0;
};