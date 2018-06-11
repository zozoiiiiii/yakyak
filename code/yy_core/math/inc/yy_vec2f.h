/************************************************************************/
/*
@author:    junliang
@brief:     œÚ¡ø¿‡
@time:      20150921
*/
/************************************************************************/
#ifndef _yy_Vec2f_h
#define _yy_Vec2f_h

#include "reflection/yy_type.h"
#include "../inc/yy_math.h"
#include <iostream>

NS_YY_BEGIN

class Vec2f
{
public:
    float x;
    float y;

    Vec2f():x(0.0f),y(0.0f){}
    Vec2f(float _x, float _y):x(_x),y(_y){}
    Vec2f(const Vec2f& vf):x(vf.x),y(vf.y){}
    void operator=(const Vec2f& vf){x=vf.x;y=vf.y;}
    float len()const;
    float len_sq()const;
    void normalize();
    void operator += (const Vec2f& vf);
};

bool operator == (const Vec2f& vf1, const Vec2f& vf2);
Vec2f operator + (const Vec2f& vf1, const Vec2f& vf2);
Vec2f operator + (const Vec2f& vf1, float val);
Vec2f operator + (float val, const Vec2f& vf1);

Vec2f operator - (const Vec2f& vf1, const Vec2f& vf2);
Vec2f operator - (const Vec2f& vf1, float val);
Vec2f operator - (float val, const Vec2f& vf1);

Vec2f operator * (const Vec2f& vf1, const Vec2f& vf2);
Vec2f operator * (const Vec2f& vf1, float val);
Vec2f operator * (float val, const Vec2f& vf1);

NS_YY_END
#endif
