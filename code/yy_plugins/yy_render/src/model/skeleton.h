/************************************************************************/
/* 
@author:    junliang
@brief:     Skeleton
@time:      6/23/2016
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/model/i_skeleton.h"
NS_YY_BEGIN

void InitBone(Bone* pBone);
void LoadSkeleton(Skeleton* pSkeleton, const std::string& file);

NS_YY_END