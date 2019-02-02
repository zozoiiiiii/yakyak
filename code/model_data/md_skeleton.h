/************************************************************************/
/* 
@author:    junliang
@brief:     anim
@time:      7/21/2016
*/
/************************************************************************/
#pragma once

#include <vector>
#include <string>


struct MD_Bone
{
    std::string name;
    int parent;
    std::vector<int> children;

    // µÈ¼ÛÓÚinverseBindPose
    float x,y,z; // local tm ? world tm?
    float angle,ax,ay,az;
    float sx,sy,sz;

    MD_Bone():parent(-1){}
};


struct MD_Skeleton
{
    std::vector<MD_Bone> bones;        // parent bone is front.
};
