/************************************************************************/
/* 
@author:    junliang
@brief:     ËÄ·½ÐÎ
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "render/inc/model/i_mesh.h"
#include "components/mesh_render/comp_mesh_render.h"

class Quad : public Comp_MeshRender
{
    YY_BEGIN(Quad, "Comp_MeshRender");
    YY_END
public:
    ~Quad(){};
    virtual void OnCreate();
    virtual void OnDestroy();

    void InitModel(const std::string& strModelFile, const std::string& strModelDir);
};