/************************************************************************/
/* 
@author:    junliang
@brief:     scene
1, game obj manager
2, camera, cull

@time:      2017-6-9
*/
/************************************************************************/
#pragma once


#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"
#include "shadow_map.h"
#include "../weather/weather.h"
#include "yy_world/inc/i_scene.h"
#include "octree_scene.h"


class World;
class Scene : public IScene
{
    YY_BEGIN(Scene, IScene);
    YY_END
public:
    Scene(){}
    virtual void OnCreate(const VariantMap& args);
    virtual void OnDestroy();
    virtual void OnExcute(float sec){}

    virtual void Update(float sec);
    virtual void Render();

    void UpdateRenderContext();
    void VisitAllObjs();

    RenderContext* GetRenderCxt();
    IRender* GetRender();

    IGameObj* CreateObj(IGameObj* pParent=NULL);
    bool AddObj(IGameObj* pObj);
    bool RemoveObj(IGameObj* pObj);

    void SetMainCamera(IGameObj* pCamera){m_pMainCamera = pCamera;}
    IGameObj* GetMainCamera(){throw_assert(NULL != m_pMainCamera, "null check."); return m_pMainCamera;}

    IGameObj* FindObjByName(const std::string& name);

    virtual IGameObj* HitByRay(const YY::Ray &ray);

    // 可视对象
	virtual int GetVisibleListNum();
	virtual YY_OBJECTID GetVisibleObjByID(int index);

    // 所有对象
    virtual int GetObjListNum();
    virtual YY_OBJECTID GetObjByID(int index);

    OctreeScene* GetOctreeScene(){return m_pOctreeScene;}

    // 按序插入
    //virtual void AddRender(RenderContext* pRI);

private:
    void OnEvent_Resize(const char* name, const YY::VarList& args);

private:
    IGameObj* m_pMainCamera;
    ShadowMap* m_pShadowMap;
    Weather* m_pWeather;
    OctreeScene* m_pOctreeScene;

    RenderContext* m_pCurRenderText;
    IBatchGroup* m_pBatchGroup;

	std::vector<YY_OBJECTID> m_curRendingGameObjs;		// 当前在渲染的gameobj
};