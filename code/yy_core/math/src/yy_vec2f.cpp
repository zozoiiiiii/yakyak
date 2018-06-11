#include "../inc/yy_Vec2f.h"
#include <math.h>

NS_YY_BEGIN

float Vec2f::len()const
{
    return sqrtf(x*x + y*y);
}

float Vec2f::len_sq()const
{
    return x*x + y*y;
}

void Vec2f::normalize()
{
    float l = 1.0f / len();
    x = x * l;
    y = y * l;
}

void Vec2f::operator += (const Vec2f& vf)
{
    x += vf.x;
    y += vf.y;
}

bool operator == (const Vec2f& vf1, const Vec2f& vf2)
{
    return FloatEqual(vf1.x, vf2.x) && FloatEqual(vf1.y, vf2.y);
}

Vec2f operator + (const Vec2f& vf1, const Vec2f& vf2)
{
    return Vec2f(vf1.x+vf2.x, vf1.y+vf2.y);
}

Vec2f operator + (const Vec2f& vf1, float val)
{
    return Vec2f(vf1.x+val, vf1.y+val);
}

Vec2f operator + (float val, const Vec2f& vf1)
{
    return Vec2f(val+vf1.x, val+vf1.y);
}

Vec2f operator - (const Vec2f& vf1, const Vec2f& vf2)
{
    return Vec2f(vf1.x-vf2.x, vf1.y-vf2.y);
}

Vec2f operator - (const Vec2f& vf1, float val)
{
    return Vec2f(vf1.x-val, vf1.y-val);
}

Vec2f operator - (float val, const Vec2f& vf1)
{
    return Vec2f(val-vf1.x, val-vf1.y);
}

Vec2f operator * (const Vec2f& vf1, const Vec2f& vf2)
{
    return Vec2f(vf1.x*vf2.x, vf1.y*vf2.y);
}

Vec2f operator * (const Vec2f& vf1, float val)
{
    return Vec2f(vf1.x*val, vf1.y*val);
}

Vec2f operator * (float val, const Vec2f& vf1)
{
    return Vec2f(val*vf1.x, val*vf1.y);
}

NS_YY_END