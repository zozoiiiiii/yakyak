#include "render_system.h"



void RenderSystem::OnCreate()
{
	GetMgr()->AddExcute(this);
	m_pRender = IRender::Instance();
}

void RenderSystem::OnDestroy()
{
	GetMgr()->RemoveExcute(this);
}
