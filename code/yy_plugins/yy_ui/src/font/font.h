/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/11/14
*/
/************************************************************************/
#pragma once


#include "freetype/config/ftheader.h"
#include FT_FREETYPE_H
#include "yy_core.h"
#include "GlyphData.h"
#include "GlyphAtlas.h"

NS_YY_BEGIN
struct Character {
	uint32     TextureID;
	YY::Vec2f Size;
	YY::Vec2f Bearing;    // baseline offset
	uint32     Advance;    // distance between current origin text to next origin text
};


class Font
{
public:
	Font(unsigned int fontSize, FT_Face* face, IObjectMgr* pObjMgr);
	FT_Face *face() const;
	void setFace(FT_Face *face);
	unsigned int getCharIndex(unsigned long c);
	GlyphData getGlyphDataFromChar(unsigned long c);
	void generateAsciiMap();
	unsigned int getFontSize() const;
	bool isUnvisibleChar(unsigned long c);
	unsigned int getSpecialAdvance(unsigned long c, unsigned int baseAdvance);
	GlyphAtlas *getGlyphAtlas() const;
	void setGlyphAtlas(GlyphAtlas *glyphAtlas);
private:
	void initAsciiData(IObjectMgr* pObjMgr);
	unsigned int m_fontSize;
	FT_Face * m_face;
	GlyphAtlas * m_glyphAtlas;
	//std::map<unsigned char, Character> Characters;

};

NS_YY_END