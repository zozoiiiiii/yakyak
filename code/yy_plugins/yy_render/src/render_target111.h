/************************************************************************/
/* 
@author:    junliang
@brief:     此处不等价于dx里的rt. 参考了tangziwen的render target(v0.13 7.9.2015)
渲染一次整个场景需要用到的渲染信息， 根据此信息生成render context，然后在framebuffer上渲染出一帧。
@time:      2017-10-22
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "render/inc/i_render_target.h"

class RenderMan? RenderBody? RenderInfo? RenderData? RenderContextOwner? : public IRenderTarget
{
public:
	virtual void OnCreate();
	virtual IRes_FBO* GetFBO(){return m_pFBO;}
	virtual void SetTargetType(int nType){m_nTargetType=nType;}
	virtual int GetTargetType(){return m_nTargetType;}

	virtual void SetCamera(YY_ENTITYID camera){m_camera = camera;}
	virtual YY_ENTITYID GetCamera(){return m_camera;}

private:
	IRes_FBO* m_pFBO;
	YY_ENTITYID m_camera;
	int m_nTargetType;
};


