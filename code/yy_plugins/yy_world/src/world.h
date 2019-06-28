/************************************************************************/
/* 
@author:    junliang
@brief:     world
@time:      2017-6-9
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "scene/scene.h"
#include "yy_render/inc/i_render.h"
#include "yy_world/inc/i_world.h"

class World : public IWorld
{
    YY_BEGIN(World, IWorld);
    YY_END
public:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnExcute(float sec);

    YY_OBJECTID GetSceneID(){return m_scene_id;}
    IScene* FindScene();
    void SetSceneID(YY_OBJECTID scene_id){m_scene_id = scene_id;}
    //IEventMgr* GetEventMgr(){return m_pEventMgr;}
    //virtual IRender* GetRender(){return m_pRender;}
private:
	void onEvent_AddBatch(const char* name, const YY::VarList& args);
private:
    YY_OBJECTID m_scene_id;
    //IEventMgr* m_pEventMgr;
    //IRender* m_pRender;
};