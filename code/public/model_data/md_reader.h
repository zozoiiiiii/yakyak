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

void MDMeshRead(const std::string& file, MD_Mesh* pMesh);
void MDSkeletonRead(const std::string& file, MD_Skeleton* pSkel);
void MDAnimRead(const std::string& file, MD_Anim* pAnim);