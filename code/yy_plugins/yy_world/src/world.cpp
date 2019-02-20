#include "world.h"
#include "core/inc/yy_ObjectMgr.h"


IScene* World::GetScene()
{
    YY::BaseObject* pBaseObject = GetMgr()->Get(m_scene_id);
    bool result = pBaseObject->IsInstanceOf("Scene");
    throw_assert(result, "type check.");
    return (IScene*)pBaseObject;
}

void World::OnCreate(const VariantMap& args)
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
    IScene* pScene = GetScene();
    //pScene->Update(sec);
    //pScene->Render();

	// paint render

    // render batch groups and clear all.
    //m_pRender->RenderAll();
}


void World::onEvent_AddBatch(const char* name, const YY::VarList& args)
{
	IScene* pScene = GetScene();
	pScene->Render();
}