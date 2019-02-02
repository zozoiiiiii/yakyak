/************************************************************************/
/*
@author:  junliang
@brief:   render all objects.
@time:    2018/09/29
*/
/************************************************************************/
#pragma once

#include "core/inc/yy_entity.h"
#include "yy_render/inc/i_render.h"


class RenderSystem : public YY::System
{
    YY_BEGIN(RenderSystem, "System");
    YY_END
public:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnExcute(float sec);

private:
	IRender* m_pRender;
};