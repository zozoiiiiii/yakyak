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
    SetGlobalEntMgr(pEntMgr);
	YY_REG(Entity);
	YY_REG(Component);
	YY_REG(RenderComponent);

	YY_REG(Render);
	YY_REG(IRes);
	YY_REG(IFrameBuffer);
	YY_REG(IRenderBuffer);
	YY_REG(ITexture);
	YY_REG(IStaticTexture);
	YY_REG(IDynamicTexture);
	YY_REG(IShader);
    YY_REG(IRenderTexture);
	YY_REG(IFontTexture);
	YY_REG(FontTexture);

    YY_REG(FrameBuffer);
    YY_REG(ColorRenderBuffer);
    YY_REG(DepthRenderBuffer);
    YY_REG(StencilRenderBuffer);
    YY_REG(DepthStencilRenderBuffer);
    YY_REG(StaticTexture);
    YY_REG(DynamicTexture);
    YY_REG(ColorTexture);
    YY_REG(DepthTexture);
    YY_REG(Shader);
    YY_REG(BatchGroup);
	YY_REG(ResMgr);

	YY_REG(Vec3f);
	//YY_REG(RenderSystem);

	//pEntMgr->Create("RenderSystem");
}
