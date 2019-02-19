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
#include "item.h"

//NS_YY_BEGIN
class RectangleComponent : public ItemComponent
{
	YY_BEGIN(RectangleComponent, ItemComponent);
	YY_END
public:
	RectangleComponent();
	virtual void OnEvent(const std::string& event, const YY::VarList& args = YY::VarList());
	virtual void OnAddBatch(IBatchGroup* pBatchGroup);
	virtual void OnRender(IRender* pRender, RenderContext* pCxt);
	void Setup();
	void SetColor(unsigned int color);
	unsigned int  GetColor();
private:
	sint32 m_color;	// unsigned int and signed int has same data, different rule. here use sint32 instead of unsigned int, as reflection module not support unsigned int.

	struct Vertex
	{
		YY::Vec3f pos;
		unsigned int color;	// 0xFF0000FF   : argb
	};

	Vertex m_vertices[4];
	unsigned int m_VBO, m_EBO;
};


//NS_YY_END