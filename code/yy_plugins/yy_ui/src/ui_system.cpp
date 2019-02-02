#include "ui_system.h"

void UISystem::OnCreate()
{
	GetEventMgr()->ConnectGlobals("2DRender", fastdelegate::MakeDelegate(this, &Comp_Camera::onEvent_2DRender));
}

void UISystem::OnDestroy()
{

}

void UISystem::OnExcute(float sec)
{

}

void UISystem::onComponentAdded(YY_OBJECTID id)
{

}

void UISystem::onComponentRemoved(YY_OBJECTID id)
{

}

