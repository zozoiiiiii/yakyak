/************************************************************************/
/* 
@author:    junliang
@brief:     texture
@time:      2017-9-21
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_render/inc/res/i_texture.h"

class StaticTexture : public IStaticTexture
{
    YY_BEGIN(StaticTexture, IStaticTexture);
    YY_END
public:
    virtual void Generate(int w, int h, int n, const void* pData);
};

class DynamicTexture : public IDynamicTexture
{
    YY_BEGIN(DynamicTexture, IDynamicTexture);
    YY_END
public:
    virtual void Generate(int w, int h, int n, const void* pData){}
};

class ColorTexture : public IRenderTexture
{
    YY_BEGIN(ColorTexture, IRenderTexture);
    YY_END
public:
    virtual void Generate();
};



class DepthTexture : public IRenderTexture
{
    YY_BEGIN(DepthTexture, IRenderTexture);
    YY_END
public:
    virtual void Generate();
};


class FontTexture : public IFontTexture
{
	YY_BEGIN(FontTexture, IFontTexture);
	YY_END
public:
	virtual void Generate(int w, int h, int n, const void* pData);
};
