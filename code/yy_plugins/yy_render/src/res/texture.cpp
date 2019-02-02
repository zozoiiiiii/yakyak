#include "texture.h"
#include "../platform.h"
#include "yy_render/inc/i_render.h"


void StaticTexture::Generate(int w, int h, int n, const void* pData)
{
    throw_assert(NULL!=pData, "null check.");

    uint32 id = GetID();
    if (GetID())
    {
        glDeleteTextures(1, &id);
    }

    glGenTextures(1, &id);
    SetID(id);

    // make active and bind
    glBindTexture(GL_TEXTURE_2D, id);

    // turn off filtering and wrap models
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //GLenum internalFormat = GL_RGB;
    //GLenum textureFormat = GL_RGB;
    //GLenum type = GL_UNSIGNED_SHORT_5_6_5;
    GLenum internalFormat = GL_RGBA;
    GLenum textureFormat = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;

    switch(n)
    {
    case TEX_FORMAT_rgb:
        {
            internalFormat = GL_RGB;
            textureFormat = GL_RGB;
            type = GL_UNSIGNED_BYTE;
        }
        break;
    case TEX_FORMAT_rgb_alpha:
        {
            internalFormat = GL_RGBA;
            textureFormat = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
        }
        break;
    default:
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, textureFormat, type, pData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ColorTexture::Generate()
{
    int nWidth=0;
    int nHeight=0;
    if(GetSizeType() == TEX_TYPE_DEVICE)
    {
        nWidth = GetRender()->GetDeviceWidth();
        nHeight = GetRender()->GetDeviceHeight();
    }
    else
    {
        nWidth = GetWidth();
        nHeight = GetHeight();
    }

    uint32 id = GetID();
    if (GetID())
    {
        glDeleteTextures(1, &id);
    }

    glGenTextures(1, &id);
    SetID(id);

    // make active and bind
    glBindTexture(GL_TEXTURE_2D, id);

    // turn off filtering and wrap models
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// changed on android
    //GLenum internalFormat = GL_RGB;
    //GLenum textureFormat = GL_RGB;
    //GLenum type = GL_UNSIGNED_SHORT_5_6_5;

	GLenum internalFormat = GL_RGBA;
	GLenum textureFormat = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;

	switch (GetFormat())
	{
	case TEX_FORMAT_rgb_alpha:
	{
		internalFormat = GL_RGBA;
		textureFormat = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
	}
	break;
	case TEX_FORMAT_rgb:
	{
		internalFormat = GL_RGB;
		textureFormat = GL_RGB;
		type = GL_UNSIGNED_BYTE;
	}
	break;
	default:
		break;
	}
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, nWidth, nHeight, 0, textureFormat, type, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DepthTexture::Generate()
{


    int nWidth=0;
    int nHeight=0;
    if(GetSizeType() == TEX_TYPE_DEVICE)
    {
        nWidth = GetRender()->GetDeviceWidth();
        nHeight = GetRender()->GetDeviceHeight();
    }
    else
    {
        nWidth = GetWidth();
        nHeight = GetHeight();
    }

    uint32 id = GetID();
    if (GetID())
    {
        glDeleteTextures(1, &id);
    }

    glGenTextures(1, &id);
    SetID(id);

    // make active and bind
    glBindTexture(GL_TEXTURE_2D, id);

    // turn off filtering and wrap models
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum internalFormat = GL_DEPTH_COMPONENT;
    GLenum textureFormat = GL_DEPTH_COMPONENT;

    // egl EGL_DEPTH_SIZE 24 use GL_UNSIGNED_INT, 16 use GL_UNSIGNED_SHORT
    //GLenum type = GL_UNSIGNED_INT;
    GLenum type = GL_FLOAT;

    switch(GetFormat())
    {
    case TEX_FORMAT_DEPTH_DEFAULT:
        {
            internalFormat = GL_DEPTH_COMPONENT;
            textureFormat = GL_DEPTH_COMPONENT;
            type = GL_FLOAT;
        }
        break;
    default:
        break;
    }

	// android test
	//type = GL_UNSIGNED_SHORT;

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, nWidth, nHeight, 0, textureFormat, type, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}



void FontTexture::Generate(int w, int h, int n, const void* pData)
{
	throw_assert(NULL != pData, "null check.");

	uint32 id = GetID();
	if (GetID())
	{
		glDeleteTextures(1, &id);
	}

	glGenTextures(1, &id);
	SetID(id);

	// make active and bind
	glBindTexture(GL_TEXTURE_2D, id);

	// turn off filtering and wrap models
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//GLenum internalFormat = GL_RGB;
	//GLenum textureFormat = GL_RGB;
	//GLenum type = GL_UNSIGNED_SHORT_5_6_5;
	GLenum internalFormat = GL_RGBA;
	GLenum textureFormat = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;

	switch (n)
	{
	case TEX_FORMAT_rgb:
	{
		// m_size += width * height * 3;
		internalFormat = GL_RGB;
		textureFormat = GL_RGB;
		type = GL_UNSIGNED_BYTE;
	}
	break;
	case TEX_FORMAT_rgb_alpha:
	{
		internalFormat = GL_RGBA;
		textureFormat = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
	}
	break;
	case TEX_FORMAT_RGBA4444_UNORM:
	{
		//m_size += width * height * 2;

		internalFormat = GL_RGBA;
		textureFormat = GL_RGBA;
		type = GL_UNSIGNED_SHORT_4_4_4_4;
	}
	break;
	default:
		break;
	}

	//glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, textureFormat, type, pData);

	// GL_LUMINANCE_ALPHA is a format with 2 bytes per pixel.  1 byte: r,g,b; 2 byte: a
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pData);
	glBindTexture(GL_TEXTURE_2D, 0);
}