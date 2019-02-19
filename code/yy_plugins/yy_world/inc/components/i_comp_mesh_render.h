/************************************************************************/
/* 
@author:    junliang
@brief:     mesh render
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include <vector>
#include "yy_core.h"
#include "../i_game_obj.h"


class IComp_MeshRender : public GameObjComponent
{
public:
    virtual void SetMesh(const std::string& path, const std::string& mesh_file, const std::string& Skeleton_file=std::string()) = 0;
    virtual int AddAnim(const std::string& anim_file, float fEnterTime=0.0f, float fLeaveTime=0.0f) = 0;
    virtual void SetDefaultAnim(int nAnimIndex) = 0;
};