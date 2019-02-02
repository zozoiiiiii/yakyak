/************************************************************************/
/* 
@author:    junliang
@brief:     anim
@time:      7/21/2016
*/
/************************************************************************/
#pragma once

#include <vector>

struct MD_KeyFrame
{
    float time;
    float x,y,z;
    float sx,sy,sz;
    float angle,ax,ay,az;
};

struct MD_Track
{    
    std::vector<MD_KeyFrame> keys;
};

struct MD_Anim
{
    std::vector<MD_Track> tracks;            // 有多少根骨骼，就有多少个track。骨骼索引值一一对应
    int frame_num;            // 帧数
    float time_length;
};
