/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/11/14
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_render/inc/res/i_texture.h"
#include "GlyphData.h"
#include <vector>
#include <map>

class GlyphAtlas;
struct GlyphAtlasNode{
    GlyphAtlasNode();
    GlyphData m_data;
    int x,y;
    YY::Vec2f UV(float u,float v);
    GlyphAtlas * parent;
    float U(float fakeU);
    float V(float fakeV);
};

class GlyphAtlas
{
public:
    GlyphAtlas();
    void addGlyphData(GlyphData data);
    void generate();
    void test();
    void generateGLTexture(IObjectMgr* pObjMgr);
    int height() const;
	IFontTexture *texture() const;
    GlyphAtlasNode * getNode(unsigned long c);
    void cleanUp();
private:
    std::vector<GlyphData > m_glyphList;
	IFontTexture * m_texture;
    std::map<unsigned long ,GlyphAtlasNode *>m_glyphMap;
    unsigned char * m_buffer;
    int m_height;

	// the biggest cell width, height
    unsigned int cellWidth,cellHeight;
};
