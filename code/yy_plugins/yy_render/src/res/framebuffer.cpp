#include <stdio.h>
#include "framebuffer.h"
#include "../platform.h"

void FrameBuffer::Open()
{
    // frame buffer object.
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBuffer::Close()
{
    if(m_fbo)
        glDeleteFramebuffers(1, &m_fbo);
}

void FrameBuffer::AttachTexture(ITexture* pTexture, int nAttachment)
{
	int nType = AT_UNKNOWN;
	switch(nAttachment)
	{
	case AT_DEPTH:
		nType = GL_DEPTH_ATTACHMENT;
		break;
	case AT_COLOR:
		nType = GL_COLOR_ATTACHMENT0;
		break;
	default:
		break;
	}

	throw_assert(nType != AT_UNKNOWN, "attachment type error.")

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, nType, GL_TEXTURE_2D, pTexture->GetID(), 0);


    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        throw_assert(false, "check.");
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::AttackRenderBuffer(IRenderBuffer* pRenderBuffer, int nAttachment)
{
    int nType = AT_UNKNOWN;
    switch(nAttachment)
    {
    case AT_DEPTH:
        nType = GL_DEPTH_ATTACHMENT;
        break;
    case AT_COLOR:
        nType = GL_COLOR_ATTACHMENT0;
        break;
    default:
        break;
    }

    throw_assert(nType != AT_UNKNOWN, "attachment type error.")

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, nType, GL_RENDERBUFFER, pRenderBuffer->GetID());
}

void FrameBuffer::BindForWriting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}
//
//
//void FrameBuffer::BindForReadingDepth(int index)
//{
//    glActiveTexture(GL_TEXTURE0 + index);
//    glBindTexture(GL_TEXTURE_2D, m_depthTex);
//}



