/************************************************************************/
/* 
@author:    junliang
@brief:     render


render process:
1, collect all render objects, cut
2, shadowmap pass----------rt1
3, geometry pass-----------rt1 -> rt2
4, post pass---------------rt2 -> rt3
5, render texture to screen.
@time:      2017-6-15
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "res/i_res.h"
#include "res/i_framebuffer.h"
#include "res/i_renderbuffer.h"
#include "res/i_texture.h"
#include "res/i_shader.h"
#include "res/i_res_mgr.h"

#include "model/i_mesh.h"
#include "model/i_anim.h"
#include "model/i_model_res.h"

#include "i_render_context.h"
#include "i_batch.h"
#include "core/inc/yy_entity.h"


#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>
class IRender : public YY::BaseObject
{
public:
	static IRender* Instance()
	{
		BaseObject* pObject = GetGlobalEntMgr()->FindGlobalObject("Render");
		if (NULL == pObject)
		{
			BaseObject* pBaseObject = GetGlobalEntMgr()->Create("Render");
			GetGlobalEntMgr()->SetGlobal("Render", pBaseObject->GetID());
			pObject = pBaseObject;
		}

		return (IRender*)pObject;
	}

    virtual IResMgr* GetResMgr() = 0;
	virtual IModelRes* GetModelResMgr() = 0;

	virtual IBatchGroup* GetUIBatchGroup() = 0;
    virtual void AddGroup(IBatchGroup* pBatchGroup) = 0;
    virtual void RenderAll() = 0;

    virtual void SetResPath(const std::string& res) = 0;
    virtual std::string GetResPath() = 0;	// "...\\res\\";

    // device(glviewport)
    virtual void SetDeviceWidth(int w) = 0;
    virtual int GetDeviceWidth() = 0;
    virtual void SetDeviceHeight(int h) = 0;
    virtual int GetDeviceHeight() = 0;

    // window
    virtual void SetWinWidth(int w) = 0;
    virtual int GetWinWidth() = 0;
    virtual void SetWinHeight(int h) = 0;
    virtual int GetWinHeight() = 0;

    virtual void CreateDevice() = 0;
	virtual void ResetDevice() = 0;

	// by default, gl_position origin is in the middle, x,y between [-1, 1], so here need x1/width, y1/height, to fit [-1,1], then invert bottom with top
	// https://stackoverflow.com/questions/25416820/set-origin-to-top-left-corner-of-screen-in-opengl-es-2
	virtual Mat4f GetTopLeftMatrix() = 0;

	virtual void RenderTexture(IRenderTexture* pRes) = 0;
};