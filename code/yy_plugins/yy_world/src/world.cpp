#include "world.h"
#include "core/inc/yy_ObjectMgr.h"


IScene* World::FindScene()
{
    YY::BaseObject* pBaseObject = GetMgr()->Find(m_scene_id);
	if (!pBaseObject)
		return nullptr;

    bool result = pBaseObject->IsInstanceOf("Scene");
	if (!result)
		return nullptr;
    return (IScene*)pBaseObject;
}

void World::OnCreate()
{
    //m_pEventMgr = (IEventMgr*)GetMgr()->Create("EventMgr");
    //m_pRender = (IRender*)GetMgr()->Create("Render");
	GetMgr()->AddExcute(this);
	GetEventMgr()->ConnectGlobals("AddBatch_VisObjs", fastdelegate::MakeDelegate(this, &World::onEvent_AddBatch));
}

void World::OnDestroy()
{
    GetMgr()->RemoveExcute(this);
}

void World::OnExcute(float sec)
{
    // add batch groups
    IScene* pScene = FindScene();
    //pScene->Update(sec);
    //pScene->Render();

	// paint render

    // render batch groups and clear all.
    //m_pRender->RenderAll();
}


void World::onEvent_AddBatch(const char* name, const YY::VarList& args)
{
	IScene* pScene = FindScene();
	if(pScene)
		pScene->Render();
}