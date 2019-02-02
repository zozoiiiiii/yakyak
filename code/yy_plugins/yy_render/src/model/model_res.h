/************************************************************************/
/* 
@author:  junliang
@brief:   
@time:    2017/7/14
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/model/i_model_res.h"
#include "yy_render/inc/i_render.h"
class ModelRes : public IModelRes
{
public:
    static ModelRes* Instance();
    void SetRender(IRender* pRender){m_pRender = pRender;}
    virtual YY::IMesh* CreateMesh();
	virtual YY::IMesh* LoadMeshFile(const std::string& path, const std::string& file);
	virtual YY::Skeleton* LoadSkelFile(const std::string& file);
	virtual YY::IAnim* LoadAnimFile(YY::Skeleton* skel, const std::string& file);

    std::string GetResPath(){return m_pRender->GetResPath();}
private:
    IRender* m_pRender;
};