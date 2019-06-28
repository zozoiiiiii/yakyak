/************************************************************************/
/* 
@author:    junliang
@brief:     shadow map: dynamic shadow(��̬��Ӱͼ)
http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/


pssm(Parallel Split Shadow Map��ƽ�зָ���Ӱͼ����һ�ָ��ݾ���Զ�����ö�����������Ⱦ��Ӱ�ķ���

@time:      2017-6-16
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_render/inc/i_render.h"

class Scene;
struct DirLight;
class ShadowMap : public YY::BaseObject
{
    YY_BEGIN(ShadowMap, "BaseObject");
    YY_END
public:
    virtual void OnCreate();
    virtual void OnDestroy(){}
    void EnableShadow(bool flag){m_bEnableShadow=flag;}
    bool GetEnable(){return m_bEnableShadow;}

    void GenerateShadowMap(std::vector<YY_OBJECTID>& objs, RenderContext* pRenderContext);
    ITexture* GetDepthTexture(){return m_pDepthTexture;}
    IFrameBuffer* GetFrameBuffer(){return m_pFrameBuffer;}
private:
    IRender* m_pRender;
    IFrameBuffer* m_pFrameBuffer;
	ITexture* m_pDepthTexture;

    bool m_bEnableShadow;              // ��Ӱ�Ƿ���Ч
};