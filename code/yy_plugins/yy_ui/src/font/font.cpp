#include "font.h"

NS_YY_BEGIN



Font::Font(unsigned int fontSize, FT_Face* face, IObjectMgr* pObjMgr)
{
	m_face = face;
	m_fontSize = fontSize;


	//initAsciiData(pObjMgr);
	//generateAsciiMap();
}

FT_Face *Font::face() const
{
	return m_face;
}

void Font::setFace(FT_Face *face)
{
	m_face = face;
}

unsigned int Font::getCharIndex(unsigned long c)
{
	return FT_Get_Char_Index(*m_face, c);
}

GlyphData Font::getGlyphDataFromChar(unsigned long c)
{
	FT_Load_Char(*m_face, c, FT_LOAD_DEFAULT | FT_LOAD_RENDER);
	GlyphData g;
	g.initFromFace(m_face, c);
	return g;
}

void Font::generateAsciiMap()
{
	GlyphAtlas atlas;
	for (int i = 0; i < 128; i++)
	{
		atlas.addGlyphData(getGlyphDataFromChar(i));
	}
	atlas.generate();
	atlas.test();
}

unsigned int Font::getFontSize() const
{
	return m_fontSize;
}

bool Font::isUnvisibleChar(unsigned long c)
{
	if (c < 32)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Font::initAsciiData(IObjectMgr* pObjMgr)
{
	m_glyphAtlas = new GlyphAtlas();
	for (int i = 0; i < 128; i++)
	{
		m_glyphAtlas->addGlyphData(getGlyphDataFromChar(i));
	}
	m_glyphAtlas->generate();
	m_glyphAtlas->generateGLTexture(pObjMgr);
}

GlyphAtlas *Font::getGlyphAtlas() const
{
	return m_glyphAtlas;
}

void Font::setGlyphAtlas(GlyphAtlas *glyphAtlas)
{
	m_glyphAtlas = glyphAtlas;
}



















NS_YY_END