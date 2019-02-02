/************************************************************************/
/* 
@author:  junliang
@brief:   frame buffer
@time:    2017/10/21
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "i_res.h"
#include "i_renderbuffer.h"
#include "i_texture.h"


//nAttachment: GL_DEPTH_ATTACHMENT; GL_COLOR_ATTACHMENT0
enum AttachType
{
	AT_UNKNOWN,
	AT_DEPTH,
	AT_COLOR,
};

class IFrameBuffer : public IRes
{
	YY_VIRTUAL_BEGIN(IFrameBuffer, IRes);
	YY_END
public:
	virtual void Open() = 0;
    virtual void Close() = 0;

	//virtual void SetSize(int w, int h) = 0;
// 
// 	//////////////////texture
// 	virtual void SetColorTexture(IColorTexture* pTexture) = 0;
// 	virtual IColorTexture* GetColorTexture() = 0;
// 
// 	virtual void SetDepthTexture(IDepthTexture* pTexture) = 0;
// 	virtual IDepthTexture* GetDepthTexture() = 0;
// 
// 	///////////////////rbo
// 	virtual void SetColorRBO(IColorRenderBuffer* pRes) = 0;
// 	virtual IColorRenderBuffer* GetColorRBO() = 0;
// 
// 	virtual void SetDepthRBO(IDepthRenderBuffer* pRes) = 0;
// 	virtual IDepthRenderBuffer* GetDepthRBO() = 0;
// 
// 	virtual void SetStencilRBO(IStencilRenderBuffer* pRes) = 0;
// 	virtual IStencilRenderBuffer* GetStencilRBO() = 0;

	virtual void BindForWriting() = 0;

	//AttachType
	virtual void AttachTexture(ITexture* pTexture, int nAttachment) = 0;
	virtual void AttackRenderBuffer(IRenderBuffer* pRes, int nAttachment) = 0;
};


