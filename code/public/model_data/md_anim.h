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
    std::vector<MD_Track> tracks;            // �ж��ٸ����������ж��ٸ�track����������ֵһһ��Ӧ
    int frame_num;            // ֡��
    float time_length;
};
