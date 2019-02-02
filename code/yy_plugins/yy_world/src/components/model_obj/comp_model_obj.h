/************************************************************************/
/* 
@author:    junliang
@brief:     obj类型的模型
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "render/inc/model/i_mesh.h"
#include "components/mesh_render/comp_mesh_render.h"

class Comp_Model_Obj : public Comp_MeshRender
{
    YY_BEGIN(Comp_Model_Obj, "Comp_MeshRender");
    YY_END
public:
    ~Comp_Model_Obj(){};
    virtual void OnCreate();
    virtual void OnDestroy();

    void InitModel(const std::string& strModelFile, const std::string& strModelDir);
};