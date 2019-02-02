#include "render_target.h"



void RenderTarget::OnCreate()
{
	m_pFBO = (IRes_FBO*)GetMgr()->Create("Res_FBO");
}