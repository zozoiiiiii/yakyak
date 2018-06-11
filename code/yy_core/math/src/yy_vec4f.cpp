#include "../inc/yy_Vec4f.h"
#include <math.h>
#include <iostream>

NS_YY_BEGIN

Vec4f Vec4f::Cross(const Vec4f& v1, const Vec4f& v2)
{
    Vec4f result;
    result.x = y*(v1.z*v2.w - v2.z*v1.w) 
        - z*(v1.y*v2.w - v2.y*v1.w)
        + w*(v1.y*v2.z - v1.z*v2.y);

    result.y =       -(x*(v1.z*v2.w - v2.z*v1.w)
        - z*(v1.x*v2.w - v2.x*v1.w)
        + w*(v1.x*v2.z - v2.x*v1.z));

    result.z =       x*(v1.y*v2.w - v2.y*v1.w)
        -y*(v1.x*v2.w - v2.x*v1.w)
        +w*(v1.x*v2.y - v2.x*v1.y);

    result.w =       -(x*(v1.y*v2.z - v2.y*v1.z)
        - y*(v1.x*v2.z - v2.x*v1.z)
        + z*(v1.x*v2.y - v2.x*v1.y));
    return result;
}




NS_YY_END
