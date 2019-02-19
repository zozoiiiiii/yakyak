/************************************************************************/
/*
@author:  junliang
@brief:   just like sprite
@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/i_batch.h"
#include "yy_render/inc/i_render.h"
#include "item.h"

//NS_YY_BEGIN
class ITexture;
class ImageComponent : public ItemComponent
{
    YY_BEGIN(ImageComponent, ItemComponent);
    YY_END


public:
	ImageComponent();
	virtual void OnEvent(const std::string& event, const YY::VarList& args = YY::VarList());
	virtual void OnAddBatch(IBatchGroup* pBatchGroup);
	virtual void OnRender(IRender* pRender, RenderContext* pCxt);

	void SetImageSource(const std::string& url);
private:
	void Setup();
private:
	struct Vertex
	{
		YY::Vec3f pos;
		YY::Vec2f tex_coords;
	};

	Vertex m_vertices[4];
	unsigned int m_VBO, m_EBO;
	ITexture* m_pTexture;
};
//NS_YY_END