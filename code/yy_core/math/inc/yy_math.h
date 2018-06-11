/************************************************************************/
/*
@author:    junliang
@brief:     math
@time:      20150929
*/
/************************************************************************/
#ifndef _yy_math_h_
#define _yy_math_h_

#include "core/inc/yy_exception.h"
#include <math.h>

#ifndef YY_PI
#define YY_PI 3.1415926535897932384626433832795f
#endif

#ifndef YY_FLT_EPSILON
//#define yy_flt_epsilon     1.192092896e-07F
#define YY_FLT_EPSILON     1.192092896e-04F
#endif

#define ToRadian(x) (float)(((x) * YY_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / YY_PI))

// �ж������������Ƿ����Ϊ���
inline bool FloatEqual(float f1, float f2)
{
    return (f1 <= (f2 + YY_FLT_EPSILON)) && (f1 >= (f2 - YY_FLT_EPSILON));
}


// �߶�������Ƿ��ཻ
inline bool RayAABB(float x1,float y1, float x2, float y2, float x, float y, float w, float h)
{
    // ������֪�����ֱ�߷���ax+by+c=0
    float a,b,c;
    a=y2-y1;
    b=x1-x2;
    c=x2*y1-x1*y2;

    // ��������ĸ����Ƿ���ֱ����
    float t1,t2,t3,t4;
    t1=a*x+b*y+c;
    t2=a*x+b*(y+h)+c;
    t3=a*(x+w)+b*(y+h)+c;
    t4=a*(x+w)+b*y+c;

    // �������߶ε��Ҳ�
    if(t1>0&&t2>0&&t3>0&&t4>0)
        return false;

    // �������߶ε����
    if(t1<0&&t2<0&&t3<0&&t4<0)
        return false;

    return true;
}

#endif