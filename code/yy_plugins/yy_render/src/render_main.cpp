/************************************************************************/
/*
@authro:    junliang
@brief:     main
@time:      20140404
*/
/************************************************************************/
#include "yy_core.h"
#include "render.h"
#include "res/framebuffer.h"
#include "res/renderbuffer.h"
#include "res/texture.h"
#include "res/shader.h"
#include "res/res_mgr.h"
#include "batch/batch_group.h"
//#include "render_system.h"

// dll export api
#ifdef _WIN32
extern"C" __declspec(dllexport) void dll_main(YY::IObjectMgr* pEntMgr);
#elif __ANDROID__
//extern"C"  void dll_main(YY::IObjectMgr* pEntMgr);
#elif __APPLE__
#endif

void plugin_main_render(YY::IObjectMgr* pEntMgr)
{
	IReflectionMgr* pReflectionMgr = pEntMgr->GetReflectionMgr();
	YY_REG(pReflectionMgr, Entity);
	YY_REG(pReflectionMgr, Component);
	YY_REG(pReflectionMgr, RenderComponent);

	YY_REG(pReflectionMgr, Render);
	YY_REG(pReflectionMgr, IRes);
	YY_REG(pReflectionMgr, IFrameBuffer);
	YY_REG(pReflectionMgr, IRenderBuffer);
	YY_REG(pReflectionMgr, ITexture);
	YY_REG(pReflectionMgr, IStaticTexture);
	YY_REG(pReflectionMgr, IDynamicTexture);
	YY_REG(pReflectionMgr, IShader);
    YY_REG(pReflectionMgr, IRenderTexture);
	YY_REG(pReflectionMgr, IFontTexture);
	YY_REG(pReflectionMgr, FontTexture);

    YY_REG(pReflectionMgr, FrameBuffer);
    YY_REG(pReflectionMgr, ColorRenderBuffer);
    YY_REG(pReflectionMgr, DepthRenderBuffer);
    YY_REG(pReflectionMgr, StencilRenderBuffer);
    YY_REG(pReflectionMgr, DepthStencilRenderBuffer);
    YY_REG(pReflectionMgr, StaticTexture);
    YY_REG(pReflectionMgr, DynamicTexture);
    YY_REG(pReflectionMgr, ColorTexture);
    YY_REG(pReflectionMgr, DepthTexture);
    YY_REG(pReflectionMgr, Shader);
    YY_REG(pReflectionMgr, BatchGroup);
	YY_REG(pReflectionMgr, ResMgr);

	YY_REG(pReflectionMgr, Vec3f);
	//YY_REG(RenderSystem);

	//pEntMgr->Create("RenderSystem");
}
