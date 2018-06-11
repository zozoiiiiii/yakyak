#include "../inc/yy_math_func.h"
#include <math.h>
#include <iostream>
#include "../inc/yy_Vec3f.h"
#include "../inc/yy_Vec2f.h"
#include "../inc/yy_Vec4f.h"
#include "../inc/yy_Mat4f.h"


#ifndef min
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

NS_YY_BEGIN


#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif

void Swap(float &a,float &b) 
{
    float t;
    t=a;
    a=b;
    b=t;
}

int Intersecttri(
                 const Vec3f& v1,const Vec3f& v2,const Vec3f& v3,
                 const Vec3f& raypos,const Vec3f& raydir,
                 float& u,float& v,float& dist)
{
    // find vectors for two edges sharing vert0  
    Vec3f edge1 = v2-v1;

    Vec3f edge2 = v3-v1;

    // begin calculating determinant - also used to calculate u parameter   
    Vec3f pvec = raydir.cross(edge2);

    // if determinant is near zero, ray lies in plane of triangle  
    float det = edge1.dot(pvec);
    Vec3f tvec; 

    if (det > 0)   
    {    
        tvec = raypos - v1;
    }  
    else   
    {      
        tvec = v1 - raypos;
        det = -det; 
    }

    if (det < 0.0001f) 
    {
        return false; 
    }

    // calculate u parameter and test bounds  
    u = tvec.dot(pvec);

    if ((u < 0.0f) || (u > det))
    {
        return false;  
    }

    // prepare to test v parameter    
    Vec3f qvec = tvec.cross(edge1);

    // calculate v parameter and test bounds  
    v = raydir.dot(qvec);

    if ((v < 0.0f) || ((u + v) > det))  
    {
        return false;   
    }

    // calculate t, scale parameters, ray intersects triangle  
    dist = edge2.dot(qvec);

    float finvdet = 1.0f / det;  

    dist *= finvdet;  
    u *= finvdet;   
    v *= finvdet;   

    return true;
}



float Distance(float x1, float y1, float x2, float y2)
{
    float sx = x1 - x2;
    float sy = y1 - y2;

    return ::sqrt(sx * sx + sy * sy);
}

float Distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float sx = x1 - x2;
    float sy = y1 - y2;
    float sz = z1 - z2;

    return ::sqrt(sx * sx + sy * sy + sz * sz);
}


float NormalizeAngle(float angle)
{
    float value = ::fmod(angle, YY_PI*2);
    if (value < 0)
    {
        value = value + YY_PI*2;
    }

    if (value < YY_FLT_EPSILON && ::fabs(angle) > YY_PI)
    {
        value = YY_PI*2;
    }
    return value;
}

float GetAngle(float x1, float z1, float x2, float z2)
{
    float sx = x2 - x1;
    float sz = z2 - z1;
    float dist = ::sqrt(sx * sx + sz * sz);
    float a = 0.0f;

    if (!FloatEqual(dist, 0.0f))
    {
        a = ::acos(sz / dist);
    }

    if (sx < 0)
    {
        a = -a;
    }

    return NormalizeAngle(a);
}


bool FloatEqual(float f1, float f2)
{
    return (f1 <= (f2 + FLT_EPSILON)) && (f1 >= (f2 - FLT_EPSILON));
}

bool FloatEqualZero(float f)
{
    return (f >= -FLT_EPSILON && f <= FLT_EPSILON);
}


// �������������α߽��ཻ�Ĵ��� https://www.zhihu.com/question/26551754/answer/33185339
bool PointInRegion(Vec2f pt, Vec2f* plist, int num)
{
    int nCross = 0;    // ���������ͳ��Ŀ������һ������������ཻ����

    for (int i = 0; i < num; i++)
    {   //���������ÿһ���ڵ�

        Vec2f p1;
        Vec2f p2;

        p1 = plist[i];
        p2 = plist[(i+1)%num];  // p1������ڵ㣬p2����һ���ڵ㣬���������Ƕ���ε�һ����
        // �����㷨����������y���������жϵ�

        if ( p1.y == p2.y )
            continue;   //�����������ˮƽ�ģ�����

        if ( pt.y < min(p1.y, p2.y)) //���Ŀ����������߶Σ�����
            continue; 

        if ( pt.y >= max(p1.y, p2.y)) //���Ŀ����������߶Σ�����
            continue; 

        //��ô�����������ǣ������p1��ˮƽ�ߣ���p2��ˮƽ�ߣ�Ŀ��������������м�
        // ��εļ��������� ��Ŀ��㣬��һ��ˮƽ�ߣ�x�������������ε�ǰ�ߵĽ���x����
        double x = (double)(pt.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
        if ( x > pt.x ) 
            nCross++; //����������ұߣ�ͳ�Ƽ�һ������ڴ�Ŀ������ҷ�һ�����ߣ�ray���������θ��ߵ��ཻ��crossing������
    } 

    if (nCross % 2 == 1)
    {
        return true; //�����������˵���ڶ������
    }
    else
    {
        return false; //�����ڶ������ �� ����
    }
}

NS_YY_END