/************************************************************************/
/*
@author:  junliang
@brief:   

use freetype library: https://learnopengl-cn.github.io/06%20In%20Practice/02%20Text%20Rendering/

@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "item.h"
#include "font/font_manager.h"
#include "yy_render/inc/model/i_mesh.h"
NS_YY_BEGIN
class Text : public Item
{
    YY_BEGIN(Text, Item);
    YY_END


public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	virtual void OnAddRender(IBatchGroup* pBatchGroup);
	virtual void OnRender(RenderContext* pCxt);

	void SetText(const std::string& str) { m_string = str; }
private:
	void genMesh();
	void initAtlas();

private:
	Font* m_pFont;
	GlyphAtlas * m_atlas;
	std::string m_string;
	int m_nMaxY;	// freetype use cartesian coordinate, here use lcd coordinate. so total string's origin is in topleft

	struct Vertex
	{
		YY::Vec3f pos;
		YY::Vec2f tex_coords;
	};

	std::vector<Vertex> m_vertices;
	std::vector<int> m_indices;
	unsigned int m_VBO, m_EBO;
};

NS_YY_END
