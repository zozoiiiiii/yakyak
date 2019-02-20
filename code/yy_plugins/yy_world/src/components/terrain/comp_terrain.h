/************************************************************************/
/* 
@author:    junliang
@brief:     terrain component

raypick计算时假设地形是一个四方形

@time:      11/28/2016
*/
/************************************************************************/
#pragma once


#include "yy_core.h"
#include "yy_render/inc/model/i_mesh.h"
#include "yy_render/inc/model/i_skeleton.h"
#include "yy_render/inc/model/i_anim.h"
#include "../../components/mesh_render/comp_mesh_render.h"


class Comp_Terrain : public Comp_MeshRender
{
    YY_BEGIN(Comp_Terrain, Comp_MeshRender);
	YY_METHOD(&Comp_Terrain::LoadTex, "LoadTex", "");
    YY_END
public:
    ~Comp_Terrain(){};
    virtual void OnCreate();
    virtual void OnDestroy();
    void LoadTex(const std::string& strHighMap, const std::string& strTex);
};