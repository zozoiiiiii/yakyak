#include "../inc/yy_Vec3f.h"
#include <math.h>
#include <iostream>

NS_YY_BEGIN

float Vec3f::len() const
{
    return sqrt(x*x + y*y + z*z);
}


float Vec3f::len_sq()const
{
    return x*x + y*y + z*z;
}

float Vec3f::dot(const Vec3f& v) const
{
    return x*v.x + y*v.y + z*v.z;
}

Vec3f Vec3f::cross(const Vec3f& v) const
{
    Vec3f result;
    result.x = y * v.z - z * v.y;
    result.y = z * v.x - x * v.z;
    result.z = x * v.y - y * v.x;
    return result;
}

Vec3f Vec3f::norm()
{
    float s = len();
    if(FloatEqual(s, 0.0f))
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        return *this;
    }

    x /= s;
    y /= s;
    z /= s;
    return *this;
}


void Vec3f::operator +=(const Vec3f& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

Vec3f operator +(const Vec3f& v1, const Vec3f& v2)
{
    return Vec3f(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

Vec3f operator -(const Vec3f& v1, const Vec3f& v2)
{
    return Vec3f(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}


Vec3f operator *(const Vec3f& v, float f)
{
    return Vec3f(v.x*f, v.y*f, v.z*f);
}


Vec3f operator *(float f, const Vec3f& v)
{
    return Vec3f(f*v.x, f*v.y, f*v.z);
}

Vec3f operator /(const Vec3f& v, float f)
{
    return Vec3f(v.x/f, v.y/f, v.z/f);
}

Vec3f operator - (const Vec3f& v)
{
    return Vec3f(-v.x,-v.y,-v.z);
}
NS_YY_END
