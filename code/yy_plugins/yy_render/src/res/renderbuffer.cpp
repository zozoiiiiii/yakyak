#include "renderbuffer.h"
#include <stdio.h>
//#include "yy_world/inc/i_world.h"
#include "../platform.h"
#include "yy_render/inc/res/i_texture.h"
#include "yy_render/inc/i_render.h"


void ColorRenderBuffer::Generate()
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
	if (id)
	{
		glDeleteRenderbuffers(1, &id);
	}

    glGenRenderbuffers(1,&id);
    SetID(id);

    glBindRenderbuffer(GL_RENDERBUFFER, id);

    GLenum internalFormat = GL_RGB565;
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, nWidth,nHeight);
}


void DepthRenderBuffer::Generate()
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
    if (id)
    {
        glDeleteRenderbuffers(1, &id);
    }

    glGenRenderbuffers(1,&id);
    SetID(id);

    glBindRenderbuffer(GL_RENDERBUFFER, id);

    GLenum internalFormat = GL_DEPTH_COMPONENT16;
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, nWidth,nHeight);
}


void StencilRenderBuffer::Generate()
{
// 	if (m_nRenderBuffer)
// 	{
// 		glDeleteRenderbuffers(1, &m_nRenderBuffer);
// 	}
// 
//     glGenRenderbuffers(1,&m_nRenderBuffer);
//     glBindRenderbuffer(GL_RENDERBUFFER, m_nRenderBuffer);
// 
//     GLenum internalFormat = GL_STENCIL_INDEX8;
//     glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, nWidth,nHeight);
}

void DepthStencilRenderBuffer::Generate()
{
// 	if (m_nRenderBuffer)
// 	{
// 		glDeleteRenderbuffers(1, &m_nRenderBuffer);
// 	}
// 
//     glGenRenderbuffers(1,&m_nRenderBuffer);
//     glBindRenderbuffer(GL_RENDERBUFFER, m_nRenderBuffer);
// 
//     GLenum internalFormat ;//= GL_DEPTH24_STENCIL8_OES;
//     glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, nWidth,nHeight);
}
