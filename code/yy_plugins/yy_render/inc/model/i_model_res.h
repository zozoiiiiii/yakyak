/************************************************************************/
/* 
@author:  junliang
@brief:   
@time:    2017/7/14
*/
/************************************************************************/
#pragma once

#include "i_mesh.h"
#include "i_anim.h"
#include "i_skeleton.h"

class IModelRes
{
public:
    virtual YY::IMesh* CreateMesh() = 0;
	virtual YY::IMesh* LoadMeshFile(const std::string& path, const std::string& file) = 0;
	virtual YY::Skeleton* LoadSkelFile(const std::string& file) = 0;
	virtual YY::IAnim* LoadAnimFile(YY::Skeleton* skel, const std::string& file) = 0;
};