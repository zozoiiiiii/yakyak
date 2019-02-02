/************************************************************************/
/* 
@author:    junliang
@brief:     frame buffer
每个rt对应一个frame buffer。
如果不用rt, 想实现一个普通场景，一个摄像机视图场景，都用默认的帧缓冲即屏幕，
则由于深度关系，无法显示正确。必须一个普通摄像机渲染到rt1上，然后渲染到屏幕；
摄像机场景渲染到rt2上，然后渲染到屏幕。这样两个场景的深度互不影响，rt2显示在rt1上。


纹理：可采样
rbo: 不可采样。 color buffer, depth buffer(深度测试需要)


@time:      2017-9-21
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_render/inc/res/i_framebuffer.h"


class FrameBuffer : public IFrameBuffer
{
    YY_BEGIN(FrameBuffer, IFrameBuffer);
    YY_END
public:
	virtual void Open();
	virtual void Close();

	//virtual void SetSize(int w, int h);

	virtual void BindForWriting();

	//nAttachment: GL_DEPTH_ATTACHMENT; GL_COLOR_ATTACHMENT0
	virtual void AttachTexture(ITexture* pTexture, int nAttachment);
	virtual void AttackRenderBuffer(IRenderBuffer* pRes, int nAttachment);
private:
    uint32 m_fbo;
};


