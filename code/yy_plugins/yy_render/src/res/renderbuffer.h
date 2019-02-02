/************************************************************************/
/* 
@author:    junliang
@brief:     render target(rt)
ÿ��rt��Ӧһ��frame buffer��
�������rt, ��ʵ��һ����ͨ������һ���������ͼ����������Ĭ�ϵ�֡���弴��Ļ��
��������ȹ�ϵ���޷���ʾ��ȷ������һ����ͨ�������Ⱦ��rt1�ϣ�Ȼ����Ⱦ����Ļ��
�����������Ⱦ��rt2�ϣ�Ȼ����Ⱦ����Ļ������������������Ȼ���Ӱ�죬rt2��ʾ��rt1�ϡ�


@time:      2017-9-21
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_render/inc/res/i_renderbuffer.h"


class ColorRenderBuffer : public IRenderBuffer
{
    YY_BEGIN(ColorRenderBuffer, IRenderBuffer);
    YY_END
public:
	virtual void Generate();
};

class DepthRenderBuffer : public IRenderBuffer
{
    YY_BEGIN(DepthRenderBuffer, IRenderBuffer);
    YY_END
public:
    virtual void Generate();
};

class StencilRenderBuffer : public IRenderBuffer
{
    YY_BEGIN(StencilRenderBuffer, IRenderBuffer);
    YY_END
public:
    virtual void Generate();
};

class DepthStencilRenderBuffer : public IRenderBuffer
{
    YY_BEGIN(DepthStencilRenderBuffer, IRenderBuffer);
    YY_END
public:
    virtual void Generate();
};