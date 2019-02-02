#include "GlyphAtlas.h"
#include <stdlib.h>
#include <cassert>

const int DATA_ROW = 10;	// max text in one row in memory
#include <stdio.h>
#include <cmath>
#include <algorithm>

#include "yy_render/inc/res/i_texture.h"
#include "yy_render/inc/res/i_res.h"

GlyphAtlas::GlyphAtlas()
{
    m_buffer = nullptr;
	m_texture = nullptr;
}

void GlyphAtlas::addGlyphData(GlyphData data)
{
    m_glyphList.push_back(data);

}

void GlyphAtlas::generate()
{
	if(m_glyphList.empty())
		return;

    cellWidth = m_glyphList.front().width;
    cellHeight = m_glyphList.front().rows;
    for (int i = 0;i<m_glyphList.size();i++)
    {
        auto glyphData = m_glyphList[i];
        cellWidth = std::max(glyphData.width,cellWidth);
        cellHeight = std::max(glyphData.rows,cellHeight);
    }
    int theCellSize = m_glyphList.size();

	// max text in one row is DATA_ROW, here return the row numbers from the input charactors.
    m_height = std::ceil(1.0 * theCellSize / DATA_ROW);

	// m_buffer in memory
	// 0,0------------------------DATA_ROW,0
	// 0,m_height-----------------DATA_ROW, m_height

    //有可能不是pot的所以需要一些调整
    //申请一块足够放下所有GlyphData的glbuffer的(且是方形的)空间
    auto buffSize = (m_height * cellHeight) * (DATA_ROW * cellWidth) * sizeof(unsigned char);
    m_buffer = static_cast<unsigned char *>(malloc(buffSize));
	assert(m_buffer);
    memset(m_buffer,0,buffSize);
    for (int i = 0;i<m_glyphList.size();i++)
    {

		// visit the charactor, find the charactor position in m_buffer
        auto glyphData = m_glyphList[i];
        auto destCellY = i/DATA_ROW;
        auto destCellX = i %DATA_ROW;
        auto offset = (destCellX)*cellWidth +(destCellY) *DATA_ROW * (cellWidth * cellHeight);		// every charactor has cellHeight rows.
        for (int y = 0; y<glyphData.rows;y++)
        {
            for(int x = 0; x<glyphData.width;x++)
            {

                m_buffer[ offset +  (y * DATA_ROW * cellWidth + x)] = glyphData.buffer[y * glyphData.width + x];
            }
        }
        auto node =new GlyphAtlasNode();
        node->parent = this;
        node->m_data = glyphData;
        node->x = destCellX * cellWidth;
        node->y = destCellY * cellHeight;
        m_glyphMap[glyphData.m_char] = node;
    }
}

void GlyphAtlas::test()
{
    auto file = fopen("./glyph_atlas_test.txt","w");
    for(int j = 0 ;j<cellHeight * m_height;j++)
    {
        for(int i =0;i<cellWidth * DATA_ROW;i++)
        {
            if(m_buffer[j * cellWidth * DATA_ROW  + i])
            {
                fprintf(file,"#");
            }else
            {
                fprintf(file," ");
            }
        }
        fprintf(file,"\n");
    }
    fclose(file);
}

void GlyphAtlas::generateGLTexture(IObjectMgr* pObjMgr)
{
	if(m_glyphList.empty())
		return;

    auto byteWidth = DATA_ROW * cellWidth;
    auto byteHeight = m_height *cellHeight;
    auto height_pow = nextPow2(byteHeight);
    auto width_pow = nextPow2(byteWidth);

	// totally w*h pixel, every pixel has 2 bytes.
    //expand the data to opengl compactable:   one pixel has 2 bytes. 1: rbg; 2:alpha
    auto glBuffer = new unsigned char[ 2 * width_pow * height_pow];
	memset(glBuffer, 0, 2 * width_pow * height_pow);
    for(int j=0; j <height_pow;j++)
	{
        for(int i=0; i < width_pow; i++)
		{
			if(i>=byteWidth || j>=byteHeight)
			{
				// empty
				glBuffer[2*(i+j*width_pow)+1] = 0;

			}else  
			{
				glBuffer[2 * (i + j * width_pow) + 1] = m_buffer[i + byteWidth * j];
				
				// the second byte: a
				glBuffer[2*(i+j*width_pow)+1] = m_buffer[i + byteWidth*j];
			}

			// the first byte: rgb
            glBuffer[2*(i+j*width_pow)] = glBuffer[2*(i+j*width_pow)+1];
        }
    }
	

	m_texture = (IFontTexture*)pObjMgr->Create("FontTexture");
	m_texture->SetWidth(width_pow);
	m_texture->SetHeight(height_pow);

	if(0)
	{// invert the image
		int i, j;
		int channels = 2;
		for (j = 0; j * 2 < height_pow; ++j)
		{
			int index1 = j * width_pow * channels;
			int index2 = (height_pow - 1 - j) * width_pow * channels;
			for (i = width_pow * channels; i > 0; --i)
			{
				unsigned char temp = glBuffer[index1];
				glBuffer[index1] = glBuffer[index2];
				glBuffer[index2] = temp;
				++index1;
				++index2;
			}
		}
	}

	m_texture->Generate(width_pow, height_pow, TEX_FORMAT_RGBA4444_UNORM, glBuffer);
    delete [] glBuffer;
}

int GlyphAtlas::height() const
{
    return m_height;
}

IFontTexture *GlyphAtlas::texture() const
{
    return m_texture;
}

GlyphAtlasNode *GlyphAtlas::getNode(unsigned long c)
{
    return m_glyphMap[c];
}

void GlyphAtlas::cleanUp()
{
    m_glyphMap.clear();
    m_glyphList.clear();
    if(m_buffer)
    {
        free(m_buffer);
		m_buffer = nullptr;
    }
}

GlyphAtlasNode::GlyphAtlasNode()
{

}

YY::Vec2f GlyphAtlasNode::UV(float u, float v)
{
    return YY::Vec2f(U(u),V(v));
}

// find the uv in the total texture.
float GlyphAtlasNode::U(float fakeU)
{
    int width = parent->texture()->GetWidth();
    int pos = x + m_data.width*fakeU;
    return 1.0f*pos / width;
}

float GlyphAtlasNode::V(float fakeV)
{
    int height = parent->texture()->GetHeight();

	// if in this way, the base line int the top, show not okay.
	int pos = y + m_data.rows * fakeV;  //theY - m_data.rows*fakeV;
	
    return 1.0f*pos / height;

	
	/*
	does the user want me to invert the image?	
	if (flags & SOIL_FLAG_INVERT_Y)
	{
		int i, j;
		for (j = 0; j * 2 < height; ++j)
		{
			int index1 = j * width * channels;
			int index2 = (height - 1 - j) * width * channels;
			for (i = width * channels; i > 0; --i)
			{
				unsigned char temp = img[index1];
				img[index1] = img[index2];
				img[index2] = temp;
				++index1;
				++index2;
			}
		}
	}
	*/
	// do this way, and invert the texture, can fix the problem. amazing.  SOIL_FLAG_INVERT_Y
	/*fakeV = 1.0 - fakeV;
	auto theY = height - y;
	int pos = theY - m_data.rows*fakeV;
	return 1.0f*pos / height;
	*/
	

	
}
