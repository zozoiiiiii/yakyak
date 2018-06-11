/************************************************************************/
/*
@author:    junliang
@brief:     math utility func
@time:      20160323
*/
/************************************************************************/
#ifndef _yy_math_func_h_
#define _yy_math_func_h_

#include "reflection/yy_type.h"
#include "../inc/yy_math.h"
#include <iostream>
#include "../inc/yy_Vec2f.h"
#include "../inc/yy_Mat4f.h"
#include "math/inc/yy_vec3f.h"
NS_YY_BEGIN



void Swap(float &a,float &b) ;

// 射线和三角形是否相交, 返回true表示相交，dist表示射线到三角形的距离
int Intersecttri(
                 const Vec3f& v1,const Vec3f& v2,const Vec3f& v3,
                 const Vec3f& raypos,const Vec3f& raydir,
                 float& u,float& v,float& dist);


float Distance(float x1, float y1, float x2, float y2);
float Distance(float x1, float y1, float z1, float x2, float y2, float z2);

// 将-pi~pi的弧度值转换为0~2*pi的弧度值
float NormalizeAngle(float angle);

// 求两点所构成矢量的弧度值(返回0~2*pi的值，以z的正方向为0度)
float GetAngle(float x1, float z1, float x2, float z2);

// 比较两个浮点数是否可认为相等
bool FloatEqual(float f1, float f2);
bool FloatEqualZero(float f);

//点是否在多边形内
bool PointInRegion(Vec2f pt, Vec2f* plist, int num);
NS_YY_END
#endif