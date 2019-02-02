/************************************************************************/
/* 
@author:    junliang
@brief:     assimp支持的模型，暂时只考虑静态模型
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "component/game_obj.h"
#include "render/mesh.h"
#include "component/mesh_render/comp_mesh_render.h"



#define YY_COMP_MODEL_Assimp_CREATE_NAME "Comp_Model_Assimp"

class Comp_Model_Assimp : public Comp_MeshRender
{
public:
    ~Comp_Model_Assimp(){};
    virtual void OnCreate(const YY::VarList& args);
    virtual void OnDestroy();
    void InitModel(const std::string& strModelFile, const std::string& strModelDir);
};