/************************************************************************/
/* 
@author:    junliang
@brief:     model data write
@time:      11/9/2016
*/
/************************************************************************/
#pragma once
#include "md_mesh.h"
#include "md_skeleton.h"
#include "md_anim.h"

void MDMeshWrite(const std::string& file, MD_Mesh* pMesh);
void MDSkeletonWrite(const std::string& file, MD_Skeleton* pSkel);
void MDAnimWrite(const std::string& file, MD_Anim* pAnim);