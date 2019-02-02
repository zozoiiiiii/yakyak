/************************************************************************/
/* 
@author:    junliang
@brief:     

1.	b is faster
	a: 100 triangle render in 100 glDrawElements
	b: 100 triangle render in 1 glDrawElements
2.  entity can choose the render sequence

@time:      2017-9-7
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "i_render_context.h"


struct RenderContext;
class RenderComponent;
class IRender;

// class IBatch : public BaseObject
// {
// 	YY_VIRTUAL_BEGIN(IBatch, BaseObject);
// 	YY_END
// public:
// 	virtual void OnRender(RenderContext* pCxt) = 0;
// 	virtual void OnRenderBefore(RenderContext* pCxt) = 0;
// 	virtual void OnRenderAfter(RenderContext* pCxt) = 0;
// };

class IBatchGroup : public YY::BaseObject
{
public:
    // batch group render priority
    enum GroupPriority{
        GROUP_NORMAL,       // scene
        GROUP_REFLECTION,   // water
        GROUP_PROJECTION,   // shadow
    };
public:
    //virtual void AddBatch(int nPassPriority, IBatch* pBatch) = 0;
    virtual void RenderAllBatch(IRender* pRender) = 0;
    virtual void ClearAllBatch() = 0;
    virtual RenderContext* GetRenderContext() = 0;

    virtual void AddNormalBatch(RenderComponent* pBatch) = 0;
	virtual void AddTopmostBatch(RenderComponent* pBatch) = 0;
	virtual void AddGUIBatch(RenderComponent* pBatch) = 0;
};


class RenderComponent : public YY::Component
{
	YY_VIRTUAL_BEGIN(RenderComponent, Component);
	YY_END
public:
	virtual void OnAddBatch(IBatchGroup* pBatchGroup) { pBatchGroup->AddNormalBatch(this); }
	virtual void OnRender(IRender* pRender, RenderContext* pCxt) {}
	//virtual void OnRenderBefore(RenderContext* pCxt);
	//virtual void OnRenderAfter(RenderContext* pCxt);
};