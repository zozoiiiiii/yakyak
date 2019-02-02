/************************************************************************/
/* 
@author:    junliang
@brief:     Skeleton
@time:      6/23/2016
*/
/************************************************************************/
#pragma once

#include "i_mesh.h"
NS_YY_BEGIN

// �����ڵ�
struct Bone
{
    std::string name;
    int parent;
    std::vector<int> children;

    YY::Vec3f pos;
    float angle,ax,ay,az;
    YY::Vec3f scale;

    YY::Mat4f inverseBindPose;            // ����ƫ�ƾ���, �ö���λ�ô�ģ�Ϳռ�ת���������ռ�
    YY::Mat4f matrix;                    // ��ʱʹ��
};

struct Skeleton
{
    std::vector<Bone> bones;    // ���ڿ�����һ��vector�������rootbone���Ż�
};

NS_YY_END