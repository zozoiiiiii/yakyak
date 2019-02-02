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

#include "yy_render/inc/model/i_anim.h"
NS_YY_BEGIN


class Anim : public IAnim
{
public:
    void Load(YY::Skeleton* skel, const std::string& file);
    void render(IShader* sr);
public:
    void load(const std::string& file);
    void  SetPose(double time);
    void  SetBindPose();

    // 给指定骨骼和他的所有骨骼子节点，计算出骨骼矩阵
    void  EvalSubtree(int boneid,int frame,float weight = 0);

    // 计算出当前帧的骨骼位置和角度，类似于插值计算了
    KeyFrame& GetInterpolatedKey(Track &t,int frame,float weight,bool normalize=false);

    // 重新设定帧率
    void  ResampleAnimationTracks(double frames_per_second);

private:
    std::string name;
    float timeLength;        // 该动画的时间长度
    std::vector<Track>    Tracks;            // 有多少根骨骼，就有多少个Track。骨骼索引值一一对应
    int frameCount;        // 帧数


    // render
    YY::Skeleton* m_skel;
};

NS_YY_END