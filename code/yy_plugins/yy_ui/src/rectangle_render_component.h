/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/10/25
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/i_batch.h"
#include "yy_render/inc/i_render.h"
#include <vector>

//NS_YY_BEGIN
class ItemTransformComponent;
class RectangleComponent;
class RectangleRenderComponent : public RenderComponent
{
	YY_BEGIN(RectangleRenderComponent, RenderComponent);
	YY_END
public:
	RectangleRenderComponent();
	virtual void OnEvent(const std::string& event, const YY::VarList& args = YY::VarList());
	virtual void OnAddBatch(IBatchGroup* pBatchGroup);
	virtual void OnRender(IRender* pRender, RenderContext* pCxt);
	void Setup();
private:
	ItemTransformComponent* m_pTransform;
	RectangleComponent* m_pRectangle;

	struct Vertex
	{
		YY::Vec3f pos;
		unsigned int color;	// 0xFF0000FF   : argb
	};

	Vertex m_vertices[4];
	unsigned int m_VBO, m_EBO;
};


//NS_YY_END