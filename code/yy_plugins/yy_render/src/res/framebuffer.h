/************************************************************************/
/* 
@author:    junliang
@brief:     frame buffer
ÿ��rt��Ӧһ��frame buffer��
�������rt, ��ʵ��һ����ͨ������һ���������ͼ����������Ĭ�ϵ�֡���弴��Ļ��
��������ȹ�ϵ���޷���ʾ��ȷ������һ����ͨ�������Ⱦ��rt1�ϣ�Ȼ����Ⱦ����Ļ��
�����������Ⱦ��rt2�ϣ�Ȼ����Ⱦ����Ļ������������������Ȼ���Ӱ�죬rt2��ʾ��rt1�ϡ�


�����ɲ���
rbo: ���ɲ����� color buffer, depth buffer(��Ȳ�����Ҫ)


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


