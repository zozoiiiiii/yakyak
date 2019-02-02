/************************************************************************/
/*
@author:  junliang
@brief:   just like sprite
@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "item.h"

class ITexture;
class Image : public Item
{
    YY_BEGIN(Image, Item);
    YY_END


public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	virtual void OnAddRender(IBatchGroup* pBatchGroup);
	virtual void OnRender(RenderContext* pCxt);

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
