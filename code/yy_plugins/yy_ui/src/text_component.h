/************************************************************************/
/*
@author:  junliang
@brief:   

use freetype library: https://learnopengl-cn.github.io/06%20In%20Practice/02%20Text%20Rendering/

@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/i_batch.h"
#include "yy_render/inc/i_render.h"
#include "font/font_manager.h"
#include "font/font.h"
#include "yy_render/inc/model/i_mesh.h"

//NS_YY_BEGIN
class ItemTransformComponent;
class TextComponent : public RenderComponent
{
    YY_BEGIN(TextComponent, RenderComponent);
    YY_END


public:
	TextComponent();
	virtual void OnEvent(const std::string& event, const YY::VarList& args = YY::VarList());
	virtual void OnAddBatch(IBatchGroup* pBatchGroup);
	virtual void OnRender(IRender* pRender, RenderContext* pCxt);

	void SetText(const std::string& str) { m_string = str; }
private:
	void genMesh();
	void initAtlas();

private:
	ItemTransformComponent* m_pTransform;
	YY::Font* m_pFont;
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

//NS_YY_END
