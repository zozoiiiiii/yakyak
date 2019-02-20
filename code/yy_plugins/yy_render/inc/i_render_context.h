/************************************************************************/
/* 
@author:    junliang
@brief:     render context, ÀàËÆopenglµÄ×´Ì¬»ú
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
//#include "i_transform.h"
#include "yy_render/inc/res/i_shader.h"
#include "yy_render/inc/res/i_framebuffer.h"



enum RenderType
{
    RT_Normal,      // normal render
    RT_Shadow,      // shader render
    RT_Reflect,     // reflect on water.
};


struct RenderContext
{
    YY::Mat4f viewMatrix;
    YY::Mat4f projMatrix;
    IShader* pShader_ShadowMap;
    int nRenderType;

    // viewport
    //int x,y,w,h;

    IFrameBuffer* pFBO;
    IRenderTexture* pRenderTexture;
    IRenderBuffer* pDepthRenderBuffer;

    // light
    // ambient
    YY::Vec3f ambient_color;
    float ambient_intensity;

    // direction light
    YY::Vec3f directionLight_color;
    float directionLight_intensity;
    YY::Vec3f directionLight_direction;
};