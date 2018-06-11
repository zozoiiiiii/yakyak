/************************************************************************/
/*
@author:    junliang
@brief:     matrix, mvp = proj * view * model


dx:row major
opengl:column major

c++ use row major in memory layout.

row major vs column major:
////////////////////////
1, matrix in memory layout is same.
https://stackoverflow.com/questions/17717600/confusion-between-c-and-opengl-matrix-order-row-major-vs-column-major
///////////////////////

OpenGL matrices have same memory layout as directx matrices.
{ x.x x.y x.z 0 y.x y.y y.z 0 z.x z.y z.z 0 p.x p.y p.z 1 }

translate :
{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, transX, transY, transZ, 1 }
when this matrix is passed to gpu, 
glsl and hlsl think this array is column major, so f[0],f[1],f[2],f[3] is the first column.
gl_position = mvp * i_pos;

/////////////////////
2, matrix mutiplyment
////////////////////
column major: matrix * vector
row major: vector * matrix


below 3 samples is same memory layout 
1.////////////////
M11=0.0f;   M12=0.0f;   M13=0.0f;   M14=0.0f;
M21=0.0f;   M22=1.0f;   M23=0.0f;   M24=0.0f;
M31=0.0f;   M32=0.0f;   M33=1.0f;   M34=0.0f;
M41=tx;     M42=ty;     M43=tz;     M44=1.0f;
2./////////////////

f[ 0]=1.0f;	f[ 4]=0.0f;	f[ 8]=0.0f;	f[12]=tx;
f[ 1]=0.0f;	f[ 5]=1.0f;	f[ 9]=0.0f;	f[13]=ty;
f[ 2]=0.0f;	f[ 6]=0.0f;	f[10]=1.0f;	f[14]=tz;
f[ 3]=0.0f;	f[ 7]=0.0f;	f[11]=0.0f;	f[15]=1.0f;

3.///////////////////
f[ 0]=1.0f;	f[ 1]=0.0f;	f[ 2]=0.0f;	f[3]=0.0f;
f[ 4]=0.0f;	f[ 5]=1.0f;	f[ 6]=0.0f;	f[7]=0.0f;
f[ 8]=0.0f;	f[ 9]=0.0f;	f[10]=1.0f;	f[11]=0.0f;
f[12]=tx;	f[13]=ty;	f[14]=tz;	f[15]=1.0f;


1, translate
row major:            column major:
1  0  0  0            1  0  0  x
0  1  0  0            0  1  0  y
0  0  1  0            0  0  1  z
dx dy dz 1            0  0  0  1

2, rotateX
row major:                   column major:
1  0      0     0            1  0     0      0
0  cosa   sina  0            0  cosa  -sina  0
0  -sina  cosa  0            0  sina  cosa   0
0  0      0     1            0  0     0      1

3, rotateY
row major:                   column major:
cosa  0  -sina  0            cosa   0  sina  0
0     1  0      0            0      1  0     0
sina  0  cosa   0            -sina  0  cosa  0
0     0  0      1            0      0  0     1

4, rotateZ
row major:                   column major:
cosa   sina  0  0            cosa  -sina  0  0
-sina  cosa  0  0            sina  cosa   0  0
0      0     1  0            0     0      1  0
0      0     0  1            0     0      0  1

5,scale
sx  0   0   0
0   sy  0   0
0   0   sz  0
0   0   0   1

help：
HLSL中矩阵的存储及mul指令(hlsl Matrix packing order)
https://github.com/sp4cerat/Skinned-Skeletal-Animation-in-OpenGL/tree/master/include/mathlib
https://github.com/tangziwen/Cube-Engine/tree/master/CubeEngine/EngineSrc/Math
@time:      20160311
*/
/************************************************************************/
#pragma once

#include "../inc/yy_Vec2f.h"
#include "../inc/yy_Vec3f.h"
#include "../inc/yy_Vec4f.h"
#include "../inc/yy_Quaternion.h"

NS_YY_BEGIN

class Mat4f
{
public:
    union
    {
        struct {
            float        M11, M12, M13, M14;
            float        M21, M22, M23, M24;
            float        M31, M32, M33, M34;
            float        M41, M42, M43, M44;
        };
        float m[4][4];
        float m_data[16];
    };


    Mat4f();
    Mat4f(const Mat4f& m1);
    Mat4f(float _M11, float _M12, float _M13, float _M14,
        float _M21, float _M22, float _M23, float _M24,
        float _M31, float _M32, float _M33, float _M34,
        float _M41, float _M42, float _M43, float _M44);

    void Ident();
    void Set(const Mat4f& m1);
    void Set(float _M11, float _M12, float _M13, float _M14,
        float _M21, float _M22, float _M23, float _M24,
        float _M31, float _M32, float _M33, float _M34,
        float _M41, float _M42, float _M43, float _M44);

    void Scale(float sx, float sy, float sz);
    void Translate(float tx, float ty, float tz);
    void SetTranslate(float tx, float ty, float tz);
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);

    void Set(float angle,float ax,float ay,float az);

    // perspective
    void PerspFovLh(float fovY, float aspect, float zn, float zf);
    void PerspFovRh(float fovY, float aspect, float zn, float zf);
    void PerspOffCenterLh(float minX, float maxX, float minY, float maxY, float zn, float zf);
    void PerspOffCenterRh(float minX, float maxX, float minY, float maxY, float zn, float zf);
    void Frustum(float l, float r, float b, float t, float n, float f);

    // ortho
    //void OrthoRh(float w, float h, float nearz, float farz);
    void OrthoRh(float left, float right, float bottom, float top, float nearz, float farz);
    void OrthoLh(float left, float right, float bottom, float top, float nearz, float farz);


    void LookAtRh(const Vec3f& _at, const Vec3f& _up);
    void LookAtLh(const Vec3f& _at, const Vec3f& _up);

    // 行列式
    float Det();

    // support both column major and row major orded matrix(inverse(transpose(A)) = transpose(inverse(A)))
    void Inverse();

    YY::Vec3f XComponent();
    YY::Vec3f YComponent();
    YY::Vec3f ZComponent();
    YY::Vec3f PosComponent();

    void Transpose();
};

// decide the left multi or right multi, here is left multi: mvp=proj*view*model
Mat4f operator * (const Mat4f& m0, const Mat4f& m1) ;
Vec4f operator * (const Mat4f& m, const Vec4f& v);
Vec3f operator * (const Mat4f& m, const Vec3f& v);
NS_YY_END
