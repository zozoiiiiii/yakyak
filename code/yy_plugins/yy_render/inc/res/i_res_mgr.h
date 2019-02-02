/************************************************************************/
/* 
@author:    junliang
@brief:     resource manager
@time:      2017-10-27
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include <string>

class IRender;
class IRes;
class ITexture;
class IShader;
class IResMgr : public YY::BaseObject
{
public:
    virtual IRender* GetRender()=0;
    virtual IRes* CreateRes(const char* name) = 0;
    virtual ITexture* LoadTexture(const std::string& file) = 0;

    // 后期可优化，vs和fs独立出来
    virtual IShader* LoadShader(const std::string& vsh, const std::string& fsh) = 0;
    virtual void Reset() = 0;
private:
    friend class Render;
    virtual void SetRender(IRender* pRender) = 0;
};