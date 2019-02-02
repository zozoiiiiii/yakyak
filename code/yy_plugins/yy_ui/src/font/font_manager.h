/************************************************************************/
/*
@author:  junliang
@brief:   

tutorial:
	1. https://learnopengl-cn.github.io/06%20In%20Practice/02%20Text%20Rendering/
	2. cubenging by tangziwen



free type use example:
	1. init
	  FT_Init_FreeType
	  FT_New_Face
	  Ft_Set_Pixel_Size
	2. set unicode
	  FT_Select_Charmap
	3. get the glyph
	4. render

free type origin at bottom-left, but origin in my gui system is top left, so should care about this.
	https://stackoverflow.com/questions/27469321/render-freetype-text-with-flipped-ortho-difference-between-top-and-baseline-of
	https://stackoverrun.com/cn/q/6260334



@time:    2018/11/14
*/
/************************************************************************/
#pragma once

#include <map>
#include "font.h"

NS_YY_BEGIN
class FontManager
{
public:
	static FontManager* Instance();

	void Init(IObjectMgr* pObjMgr);
	Font* GetFont(std::string fontFile, unsigned int fontSize);
	Font* GetDefaultFont() const;

private:
	FT_Library * m_library;
	std::map<std::string, Font *> m_fontMap;
	Font * m_defaultFont;
	IObjectMgr* m_pObjMgr;
};

NS_YY_END