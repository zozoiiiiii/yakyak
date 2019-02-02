/************************************************************************/
/*
@author:    junliang
@brief:     向量类
@time:      20150921
*/
/************************************************************************/
#ifndef _yy_Vec3f_h
#define _yy_Vec3f_h

#include "reflection/yy_type.h"
#include "../inc/yy_math.h"
#include <iostream>
#include "reflection/yy_reflection.h"


NS_YY_BEGIN

class  Vec3f
{
    YY_BEGIN(Vec3f, "");
    YY_FIELD(&Vec3f::x, "x", "");
    YY_FIELD(&Vec3f::y, "y", "");
    YY_FIELD(&Vec3f::z, "z", "");
    YY_END
public:
    float x;
    float y;
    float z;

    Vec3f():x(0.0f),y(0.0f),z(0.0f){}
    Vec3f(float _x, float _y, float _z)
    {
        Set(_x,_y,_z);
    }

    void Set(float _x, float _y, float _z)
    {
        x=_x;
        y=_y;
        z=_z;
        if(FloatEqual(x,0.0f))
            x=0.0f;

        if(FloatEqual(y,0.0f))
            y=0.0f;
        if(FloatEqual(z,0.0f))
            z=0.0f;
    }

    void operator =(const Vec3f& v){x=v.x;y=v.y;z=v.z;}

    float len() const;
    float len_sq() const;

    // 点乘
    float dot(const Vec3f& v) const;

    // 叉乘
    Vec3f cross(const Vec3f& v) const;


    Vec3f norm();

    void operator +=(const Vec3f& v);
};

Vec3f operator +(const Vec3f& v1, const Vec3f& v2);
Vec3f operator -(const Vec3f& v1, const Vec3f& v2);
Vec3f operator *(const Vec3f& v, float f);
Vec3f operator *(float f, const Vec3f& v);
Vec3f operator *(const Vec3f& v, float f);
Vec3f operator /(const Vec3f& v, float f);
Vec3f operator - (const Vec3f& v);


NS_YY_END

using namespace YY;
YY_TYPEHELPER(Vec3f, YVT_CLASS);

#endif
