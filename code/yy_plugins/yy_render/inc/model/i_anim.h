/************************************************************************/
/*
@author:    junliang
@brief:     动画组件, 时刻给每个顶点一个变换矩阵, 变换顶点的位置和旋转

            动画类型:
                帧动画
                骨骼动画

            url:http://shiba.hpe.sh.cn/jiaoyanzu/wuli/showArticle.aspx?articleId=358&classId=4

            骨骼动画合成/过渡
            http://www.cnblogs.com/mchz/p/3654941.html
@time:      20150214
*/
/************************************************************************/
#pragma once
#include "i_mesh.h"
#include "i_skeleton.h"
NS_YY_BEGIN


struct KeyFrame
{    
    float    time;
    float angle,ax,ay,az;
    YY::Vec3f pos;
    YY::Vec3f scale;

    KeyFrame()
    {
        time=0.0f;
        angle=ax=ay=az=0.0f;
    }
};

// 一根骨骼的所有帧, 这里的帧是美术在3dmax中的帧。下面的frameCount是程序中的帧，不一样。
struct Track
{    
    std::vector<KeyFrame> keys;
};

class IAnim
{
public:
    virtual void Load(YY::Skeleton* skel, const std::string& file) = 0;
    virtual void render(IShader* sr) = 0;
};

NS_YY_END