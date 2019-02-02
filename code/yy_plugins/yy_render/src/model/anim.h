/************************************************************************/
/*
@author:    junliang
@brief:     �������, ʱ�̸�ÿ������һ���任����, �任�����λ�ú���ת

            ��������:
                ֡����
                ��������

            url:http://shiba.hpe.sh.cn/jiaoyanzu/wuli/showArticle.aspx?articleId=358&classId=4

            ���������ϳ�/����
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

    // ��ָ���������������й����ӽڵ㣬�������������
    void  EvalSubtree(int boneid,int frame,float weight = 0);

    // �������ǰ֡�Ĺ���λ�úͽǶȣ������ڲ�ֵ������
    KeyFrame& GetInterpolatedKey(Track &t,int frame,float weight,bool normalize=false);

    // �����趨֡��
    void  ResampleAnimationTracks(double frames_per_second);

private:
    std::string name;
    float timeLength;        // �ö�����ʱ�䳤��
    std::vector<Track>    Tracks;            // �ж��ٸ����������ж��ٸ�Track����������ֵһһ��Ӧ
    int frameCount;        // ֡��


    // render
    YY::Skeleton* m_skel;
};

NS_YY_END