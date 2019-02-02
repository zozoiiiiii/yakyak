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

// һ������������֡, �����֡��������3dmax�е�֡�������frameCount�ǳ����е�֡����һ����
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