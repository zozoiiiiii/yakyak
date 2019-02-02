/************************************************************************/
/* 
@author:    junliang
@brief:     resource manager
@time:      2017-10-27
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/res/i_res_mgr.h"
#include "yy_render/inc/i_render.h"

class ResMgr : public IResMgr
{
    YY_BEGIN(ResMgr, IResMgr);
    YY_END
public:
    virtual IRes* CreateRes(const char* name);
    virtual IRender* GetRender(){return m_pRender;}

    // return null is load failed.
    virtual ITexture* LoadTexture(const std::string& file);
    virtual IShader* LoadShader(const std::string& vsh, const std::string& fsh);
    virtual void Reset();
private:
    virtual void SetRender(IRender* pRender);
    ITexture* InnerLoadTexture(const std::string& file);
private:
    IRender* m_pRender;
	std::map<std::string, IShader*> m_shaders;
	std::map<std::string, ITexture*> m_textures;
};