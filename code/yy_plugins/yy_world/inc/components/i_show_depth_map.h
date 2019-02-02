/************************************************************************/
/* 
@author:    junliang
@brief:     显示深度图的obj
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "i_comp_mesh_render.h"
#include <string>

class IShowDepthMap : public IComponent
{
public:
    virtual void Load(const std::string& model_name, const std::string& model_path) = 0;
};