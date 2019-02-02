/************************************************************************/
/* 
@author:    junliang
@brief:     scene mgr
@time:      2017-7-18
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/i_world.h"
#include "yy_world/inc/i_scene.h"

class SceneMgr : public YY::BaseObject
{
    YY_BEGIN(SceneMgr, "BaseObject");
    YY_END

public:
    virtual ~SceneMgr(){}
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnExcute(float sec);

    void EnterScene(const char* scene_file);
    void EnterEmptyScene();
private:
    IWorld* m_pWorld;
    IScene* m_pScene;
};