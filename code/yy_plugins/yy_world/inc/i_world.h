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
//#include "i_event_mgr.h"
#include "yy_render/inc/i_render.h"

class IWorld : public YY::BaseObject
{
	YY_VIRTUAL_BEGIN(IWorld, BaseObject);
	YY_END
public:
    static IWorld* Instance(IObjectMgr* pObjMgr)
	{
		static IWorld* s_pWorld = nullptr;
		if (s_pWorld)
			return s_pWorld;

		BaseObject* pObject = pObjMgr->FindGlobalObject("World");
		if (NULL == pObject)
		{
			BaseObject* pBaseObject = pObjMgr->Create("World");
			pObjMgr->SetGlobal("World", pBaseObject->GetID());
			pObject = pBaseObject;
		}

		s_pWorld = (IWorld*)pObject;
		return s_pWorld;
    }

    virtual YY_OBJECTID GetSceneID() = 0;
    virtual IScene* GetScene() = 0;
    virtual void SetSceneID(YY_OBJECTID scene_id) = 0;
//    virtual IEventMgr* GetEventMgr() = 0;
    //virtual IRender* GetRender() = 0;
};