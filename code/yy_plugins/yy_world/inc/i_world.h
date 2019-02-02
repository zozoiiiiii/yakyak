/************************************************************************/
/* 
@author:    junliang
@brief:     

BaseObject manager


--------------- scene1
----------------------render context

--------------- scene2
----------------------render context

--------------- world
--------------- ---------gui manager
--------------- ---------event manager
--------------- ---------render

--------------- ---------main scene
--------------- --------------render context



@time:      2017-6-9
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "i_scene.h"
#include "i_event_mgr.h"
#include "yy_render/inc/i_render.h"

class IWorld : public YY::BaseObject
{
public:
    static IWorld* Instance()
    {
        YY_OBJECTID id= YY_INVALID_OBJECTID;
        if(GetGlobalEntMgr()->FindGlobal("World"))
        {
            id = GetGlobalEntMgr()->GetGlobal("World").GetInt64();
        }

        IWorld* pWorld = (IWorld*)GetGlobalEntMgr()->Find(id);
        if(NULL == pWorld)
        {
            IWorld* pBaseObject = (IWorld*)GetGlobalEntMgr()->Create("World");
            GetGlobalEntMgr()->SetGlobal("World", pBaseObject->GetID());
            pWorld = pBaseObject;
        }

        return pWorld;
    }

    virtual YY_OBJECTID GetSceneID() = 0;
    virtual IScene* GetScene() = 0;
    virtual void SetSceneID(YY_OBJECTID scene_id) = 0;
    virtual IEventMgr* GetEventMgr() = 0;
    virtual IRender* GetRender() = 0;
};