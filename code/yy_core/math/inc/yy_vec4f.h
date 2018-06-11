/************************************************************************/
/*
@author:    junliang
@brief:     œÚ¡ø¿‡
@time:      20150921
*/
/************************************************************************/
#ifndef _yy_Vec4f_h
#define _yy_Vec4f_h

#include "reflection/yy_type.h"
#include "../inc/yy_math.h"
#include <iostream>

NS_YY_BEGIN

class Vec4f
{
public:
    float x;
    float y;
    float z;
    float w;

    Vec4f():x(0.0f), y(0.0f), z(0.0f),w(0.0f){}
    Vec4f(const float _x, const float _y, const float _z, const float _w):x(_x),y(_y),z(_z),w(_w){}
    void operator =(const Vec4f& v){x=v.x;y=v.y;z=v.z;w=v.w;}

    Vec4f Cross(const Vec4f& v1, const Vec4f& v2);
};


NS_YY_END
#endif
