/************************************************************************/
/* 
@author:  junliang
@brief:   texture
@time:    2017/10/21
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "i_res.h"


enum 
{
    TEX_TYPE_FIXED,	 // render target size is fixed
    TEX_TYPE_DEVICE, // render target size is based on device size.
};

// texture type
enum
{
	// enum value is same as stb_image
    TEX_FORMAT_default = 0, // only used for req_comp, same as STBI_default
    TEX_FORMAT_grey       = 1,
    TEX_FORMAT_grey_alpha = 2,
    TEX_FORMAT_rgb        = 3,
	TEX_FORMAT_rgb_alpha  = 4,
	TEX_FORMAT_RGBA4444_UNORM = 5,
    

    // depth
    TEX_FORMAT_DEPTH_DEFAULT,
};

class ITexture : public IRes
{
	YY_VIRTUAL_BEGIN(ITexture, IRes);
	YY_END
public:
    void SetWidth(int w){m_nWidth=w;}
    int GetWidth(){return m_nWidth;}
    void SetHeight(int h){m_nHeight=h;}
    int GetHeight(){return m_nHeight;}
    void SetFormat(int nFormat){m_nFormat = nFormat;}
    int GetFormat(){return m_nFormat;}
    void SetSizeType(int nSizeType){m_nSizeType=nSizeType;}
    int GetSizeType(){return m_nSizeType;}
    uint32 GetID(){return m_nID;}
    void SetID(uint32 id){m_nID = id;}

    virtual void Generate(){}
private:
    int m_nWidth;
    int m_nHeight;
    int m_nSizeType;    // TEX_TYPE_DEVICE
    int m_nFormat;
    uint32 m_nID;
};



class IStaticTexture : public ITexture
{
	YY_VIRTUAL_BEGIN(IStaticTexture, ITexture);
	YY_END
public:
    virtual void Generate(int w, int h, int n, const void* pData) = 0;
};

class IDynamicTexture : public ITexture
{
	YY_VIRTUAL_BEGIN(IDynamicTexture, ITexture);
	YY_END
public:
    virtual void Generate(int w, int h, int n, const void* pData) = 0;
};

// color texture, depth texture
class IRenderTexture : public ITexture
{
    YY_VIRTUAL_BEGIN(IRenderTexture, ITexture);
    YY_END
};

class IFontTexture : public ITexture
{
	YY_VIRTUAL_BEGIN(IFontTexture, ITexture);
	YY_END
public:
	virtual void Generate(int w, int h, int n, const void* pData) = 0;
};