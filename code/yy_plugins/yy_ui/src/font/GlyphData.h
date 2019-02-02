/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/11/14
*/
/************************************************************************/
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H



unsigned int nextPow2(unsigned int num);

class Font;
struct GlyphData{
    unsigned int rows;
    unsigned int width;
    unsigned int height_pow;
    unsigned int width_pow;
    unsigned char * buffer;		// (*face)->glyph->bitmap.buffer
    unsigned char * glBuffer;
    int top;
    int left;
    int advance;
    void initFromFace(FT_Face *face, unsigned long c);
    unsigned int getSpecialAdvance(unsigned long c,unsigned int baseAdvance);
    bool isUnvisibleChar(unsigned long c);

    unsigned long m_char;
};

