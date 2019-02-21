/************************************************************************/
/* 
@author:    junliang
@brief:     
@time:      2017-6-15
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/i_render.h"
#include "platform.h"
#include "yy_render/inc/res/i_res_mgr.h"


class Render : public IRender
{
    YY_BEGIN(Render, Render);
	YY_FIELD(&Render::m_nWinWidth, "width", "");
	YY_FIELD(&Render::m_nWinHeight, "height", "");
    YY_END
public:
    virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	virtual void OnExcute(float sec);

    virtual IModelRes* GetModelResMgr();
    virtual IResMgr* GetResMgr();

	virtual IBatchGroup* GetUIBatchGroup() { return m_pUIBatchGroup; }
    virtual void AddGroup(IBatchGroup* pBatchGroup);
    virtual void RenderAll();

    virtual void SetResPath(const std::string& res){m_strResPath = res;}
    virtual std::string GetResPath(){return m_strResPath;}

    virtual void SetDeviceWidth(int w){m_nDeviceWidth = w;}
    virtual int GetDeviceWidth(){return m_nDeviceWidth;}
    virtual void SetDeviceHeight(int h){m_nDeviceHeight = h;}
    virtual int GetDeviceHeight(){return m_nDeviceHeight;}

    virtual void SetWinWidth(int w){m_nWinWidth = w;}
    virtual int GetWinWidth(){return m_nWinWidth;}
    virtual void SetWinHeight(int h){m_nWinHeight = h;}
    virtual int GetWinHeight(){return m_nWinHeight;}

    virtual void CreateDevice();
	virtual void ResetDevice();
	virtual Mat4f GetTopLeftMatrix();

	void ScreenShot(const char* name, int x, int y, int w, int h);


	// 渲染纹理到全屏
	void RenderTexture(IRenderTexture* pRes);

    // 类似unity3d的Graphics.Blit，实现后处理的一种方式: 绘制一个四方形，用mat这个材质，用src做maintex，然后先clear为black，然后渲染到target上
    //void Blit(IColorTexture* src, IColorTexture* target, Material* mat);
private:
    // foward render
    void ForwardRendering();
    void ForwardRenderPass(IBatchGroup* pGroup);

    // deferred render
    //////////////////
    //void DeferredRendering();
    //void DeferredRendering(const RenderInfo* pRI);

    void SetupQuad();
private:
	IBatchGroup* m_pUIBatchGroup;
    std::vector<IBatchGroup*> m_groups;
    YY::IMesh* m_pQuad; // 用于将纹理渲染到全屏
    IResMgr* m_pResMgr;
    std::string m_strResPath;   // 资源相对路径

    // glviewport
    int m_nDeviceWidth;
    int m_nDeviceHeight;

    // window size
    int m_nWinWidth;
    int m_nWinHeight;

	Mat4f m_topLeftMatrix;
	IFrameBuffer* m_pFBO;
};