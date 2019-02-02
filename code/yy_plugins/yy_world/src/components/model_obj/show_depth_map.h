/************************************************************************/
/* 
@author:    junliang
@brief:     显示深度图的obj
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "render/inc/model/i_mesh.h"
#include "components/mesh_render/comp_mesh_render.h"
#include "comp_model_obj.h"
#include "world/inc/components/i_show_depth_map.h"

class ShowDepthMap : public IShowDepthMap
{
    YY_BEGIN(ShowDepthMap, "ShowDepthMap");
    YY_END
public:
    virtual ~ShowDepthMap(){};
    virtual void OnCreate();
    virtual void OnDestroy();

    virtual void Load(const std::string& model_name, const std::string& model_path);
    virtual void OnRender(RenderContext* pCxt);
private:
    YY::IMesh* m_mesh;
};