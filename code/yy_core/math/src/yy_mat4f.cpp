#include "../inc/yy_Mat4f.h"
#include <string.h>
#include <math.h>
#include "../inc/yy_math.h"
#include "math/inc/yy_math_func.h"


NS_YY_BEGIN



Mat4f::Mat4f()
{
    Ident();
}

Mat4f::Mat4f(const Mat4f& m1)
{
    Set(m1);
}

Mat4f::Mat4f(float _M11, float _M12, float _M13, float _M14,
      float _M21, float _M22, float _M23, float _M24,
      float _M31, float _M32, float _M33, float _M34,
      float _M41, float _M42, float _M43, float _M44)
{
    Set(_M11, _M12, _M13, _M14,
        _M21, _M22, _M23, _M24,
        _M31, _M32, _M33, _M34,
        _M41, _M42, _M43, _M44);
}

void Mat4f::Ident()
{
    memset(m_data,0,sizeof(m_data));
    M11 = 1.0f;
    M22 = 1.0f;
    M33 = 1.0f;
    M44 = 1.0f;
}


void Mat4f::Set(const Mat4f& m1)
{
    memcpy(m_data, m1.m_data, sizeof(float)*16);
}

void Mat4f::Set(float _M11, float _M12, float _M13, float _M14,
         float _M21, float _M22, float _M23, float _M24,
         float _M31, float _M32, float _M33, float _M34,
         float _M41, float _M42, float _M43, float _M44)
{
    M11 = _M11; M12 = _M12; M13 = _M13; M14 = _M14;
    M21 = _M21; M22 = _M22; M23 = _M23; M24 = _M24;
    M31 = _M31; M32 = _M32; M33 = _M33; M34 = _M34;
    M41 = _M41; M42 = _M42; M43 = _M43; M44 = _M44;
}


void Mat4f::Scale(float sx, float sy, float sz)
{
    M11 *= sx;
    M22 *= sy;
    M33 *= sz;
}

void Mat4f::Translate(float tx, float ty, float tz)
{
    M41 += tx;
    M42 += ty;
    M43 += tz;
}

void Mat4f::SetTranslate(float tx, float ty, float tz)
{
    M41 = tx;
    M42 = ty;
    M43 = tz;
}

/*
1.0f,   0.0f,           0.0f,           0.0f,
0.0f,   cosf(angle),    sinf(angle),    0.0f,
0.0f,   -sinf(angle),   cosf(angle),    0.0f,
0.0f,   0.0f,           0.0f,           1.0f
*/
void Mat4f::RotateX(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    int i;
    for (i=0; i<4; i++) {
        float mi1 = m[i][1];
        float mi2 = m[i][2];
        m[i][1] = mi1*c + mi2*-s;
        m[i][2] = mi1*s + mi2*c;
    }
}

/*
cosf(angle),    0.0f,   -sinf(angle),   0.0f,
0.0f,           1.0f,   0.0f,           0.0f,
sinf(angle),    0.0f,   cosf(angle),    0.0f,
0.0f,           0.0f,   0.0f,           1.0f
*/
void Mat4f::RotateY(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    int i;
    for (i=0; i<4; i++) {
        float mi0 = m[i][0];
        float mi2 = m[i][2];
        m[i][0] = mi0*c + mi2*s;
        m[i][2] = mi0*-s + mi2*c;
    }
}

/*
cosf(angle),    sinf(angle),    0.0f,   0.0f,
-sinf(angle),   cosf(angle),    0.0f,   0.0f,
0.0f,           0.0f,           1.0f,   0.0f,
0.0f,           0.0f,           0.0f,   1.0f
*/
void Mat4f::RotateZ(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    int i;
    for (i=0; i<4; i++) {
        float mi0 = m[i][0];
        float mi1 = m[i][1];
        m[i][0] = mi0*c + mi1*-s;
        m[i][1] = mi0*s + mi1*c;
    }
}

void Mat4f::Set(float angle,float ax,float ay,float az)
{
    float c = cos(angle);
    float s = sin(angle);

    float t = 1.0f - c;

    Vec3f a1(ax,ay,az);
    a1.norm();

    //  if axis is not already normalised then uncomment this
    // double magnitude = math.sqrt(a1.x*a1.x + a1.y*a1.y + a1.z*a1.z);
    // if (magnitude==0) throw error;
    // a1.x /= magnitude;
    // a1.y /= magnitude;
    // a1.z /= magnitude;

    M11 = c + a1.x*a1.x*t;
    M22 = c + a1.y*a1.y*t;
    M33 = c + a1.z*a1.z*t;
    float tmp1 = a1.x*a1.y*t;
    float tmp2 = a1.z*s;
    M21 = tmp1 - tmp2;
    M12 = tmp1 + tmp2;
    tmp1 = a1.x*a1.z*t;
    tmp2 = a1.y*s;
    M31 = tmp1 + tmp2;
    M13 = tmp1 - tmp2;    
    tmp1 = a1.y*a1.z*t;
    tmp2 = a1.x*s;
    M32 = tmp1 - tmp2;
    M23 = tmp1 + tmp2;
}


void Mat4f::PerspOffCenterLh(float l, float r, float b, float t, float n, float f)
{
    M11 = 2.0f*n/(r-l);    M12 = 0.0f,               M13 = 0.0f;        M14 = 0.0f;
    M21 = 0.0f;            M22 = 2.0f*n/(t-b);       M23 = 0.0f;        M24 = 0.0f;
    M31 = (r+l)/(r-l);     M32 = (t+b)/(b-t);        M33 = f/(f-n);     1.0f;
    M41 = 0.0f;            M42 = 0.0f;               M43 = n*f/(n-f);   0.0f;
}

void Mat4f::PerspOffCenterRh(float l, float r, float b, float t, float n, float f)
{
    M11 = 2.0f*n/(r-l);     M12 = 0.0f,             M13 = 0.0f;             M14 = 0.0f;
    M21 = 0.0f;             M22 = 2.0f*n/(t-b);     M23 = 0.0f;             M24 = 0.0f;
    M31 = (r+l)/(r-l);      M32 = (t+b)/(t-b);      M33 = f/(n-f);          -1.0f;
    M41 = 0.0f;             M42 = 0.0f;             M43 = n*f/(n-f);        0.0f;
}


void Mat4f::Frustum(float l, float r, float b, float t, float n, float f)
{
     m_data[0]=2.0f*n/(r-l);     m_data[4]=0.0f;             m_data[8]=(r+l)/(r-l);      m_data[12] = 0.0f;
     m_data[1]=0.0f;             m_data[5]=2.0f*n/(t-b);     m_data[9]=(t+b)/(t-b);      m_data[13]= 0.0f;
     m_data[2]=0.0f;             m_data[6]=0.0f;             m_data[10]= -(f+n)/(f-n);   m_data[14]= -2.0f*f*n/(f-n);
     m_data[3]=0.0f;             m_data[7]=0.0f;             m_data[11]=-1.0f;            m_data[15]= 0.0f;
}



void Mat4f::OrthoLh(float l, float r, float b, float t, float n, float f)
{
    M11 = 2.0f / (r-l); M12 = 0.0f;         M13 = 0.0f;             M14 = 0.0f;
    M21 = 0.0f;         M22 = 2.0f/(t-b);   M23 = 0.0f;             M24 = 0.0f;
    M31 = 0.0f;         M32 = 0.0f;         M33 = 1.0f/(f-n);      M34 = 0.0f;
    M41 = -(r+l)/(r-l); M42 = -(t+b)/(t-b); M43 = (n)/(n-f);        M44 = 1.0f;
}

void Mat4f::OrthoRh(float l, float r, float b, float t, float n, float f)
{
    M11 = 2.0f / (r-l); M12 = 0.0f;         M13 = 0.0f;             M14 = 0.0f;
    M21 = 0.0f;         M22 = 2.0f/(t-b);   M23 = 0.0f;             M24 = 0.0f;
    M31 = 0.0f;         M32 = 0.0f;         M33 = -1.0f/(f-n);      M34 = 0.0f;
    M41 = -(r+l)/(r-l); M42 = -(t+b)/(t-b); M43 = -n/(f-n);     M44 = 1.0f;
}

// 
// void Mat4f::OrthoRh(float w, float h, float n, float f)
// {
//     M11 = 2.0f/w;       M12 = 0.0f;         M13 = 0.0f;             M14 = 0.0f;
//     M21 = 0.0f;         M22 = 2.0f/h;       M23 = 0.0f;             M24 = 0.0f;
//     M31 = 0.0f;         M32 = 0.0f;         M33 = -1.0f/(f-n);      M34 = (n)/(n-f);
//     M41 = 0.0f;         M42 = 0.0f;         M43 = 0.0f;             M44 = 1.0f;
// }


void Mat4f::PerspFovRh(float fovY, float aspect, float zn, float zf)
{
     float h = float(1.0 / tan(fovY * 0.5f));
     float w = h / aspect;
 
     M11 = w;    M12 = 0.0f; M13 = 0.0f;                  M14 = 0.0f;
     M21 = 0.0f; M22 = h;    M23 = 0.0f;                  M24 = 0.0f;
     M31 = 0.0f; M32 = 0.0f; M33 = zf / (zn - zf);        M34 = -1.0f;
     M41 = 0.0f; M42 = 0.0f; M43 = zn * (zf / (zn - zf)); M44 = 0.0f;
}

void Mat4f::PerspFovLh(float fovY, float aspect, float zn, float zf)
{
    float h = float(1.0 / tan(fovY * 0.5f));
    float w = h / aspect;
    M11 = w;    M12 = 0.0f; M13 = 0.0f;                   M14 = 0.0f;
    M21 = 0.0f; M22 = h;    M23 = 0.0f;                   M24 = 0.0f;
    M31 = 0.0f; M32 = 0.0f; M33 = zf / (zf - zn);         M34 = 1.0f;
    M41 = 0.0f; M42 = 0.0f; M43 = -zn * (zf / (zf - zn)); M44 = 0.0f;
}


void Mat4f::LookAtRh(const Vec3f& at, const Vec3f& aUp)
{
    Vec3f eye(M41, M42, M43);
    Vec3f zaxis = at - eye;
    zaxis.norm();

    Vec3f xaxis = zaxis.cross( aUp );
    xaxis.norm();

    const Vec3f yaxis = xaxis.cross( zaxis );
    M11 = xaxis.x;  M12 = xaxis.y;  M13 = xaxis.z;  M14 = 0.0f;
    M21 = yaxis.x;  M22 = yaxis.y;  M23 = yaxis.z;  M24 = 0.0f;
    M31 = -zaxis.x;  M32 = -zaxis.y;  M33 = -zaxis.z;  M34 = 0.0f;
    return;
}



void Mat4f::LookAtLh(const Vec3f& at, const Vec3f& aUp)
{
    Vec3f eye(M41, M42, M43);
    Vec3f zaxis = at - eye;
    zaxis.norm();

    Vec3f xaxis = zaxis.cross( aUp );
    xaxis.norm();

    const Vec3f yaxis = xaxis.cross( zaxis );
    M11 = xaxis.x;  M12 = yaxis.x;  M13 = zaxis.x;  M14 = 0.0f;
    M21 = xaxis.y;  M22 = yaxis.y;  M23 = zaxis.y;  M24 = 0.0f;
    M31 = xaxis.z;  M32 = yaxis.z;  M33 = zaxis.z;  M34 = 0.0f;
    return;
}


float Mat4f::Det()
{
    return
        (M11 * M22 - M12 * M21) * (M33 * M44 - M34 * M43)
        -(M11 * M23 - M13 * M21) * (M32 * M44 - M34 * M42)
        +(M11 * M24 - M14 * M21) * (M32 * M43 - M33 * M42)
        +(M12 * M23 - M13 * M22) * (M31 * M44 - M34 * M41)
        -(M12 * M24 - M14 * M22) * (M31 * M43 - M33 * M41)
        +(M13 * M24 - M14 * M23) * (M31 * M42 - M32 * M41);
}

void Mat4f::Inverse()
{
    float s = Det();
    if (s == 0.0) return;
    s = 1/s;

    Set(
        s*(M22*(M33*M44 - M34*M43) + M23*(M34*M42 - M32*M44) + M24*(M32*M43 - M33*M42)),
        s*(M32*(M13*M44 - M14*M43) + M33*(M14*M42 - M12*M44) + M34*(M12*M43 - M13*M42)),
        s*(M42*(M13*M24 - M14*M23) + M43*(M14*M22 - M12*M24) + M44*(M12*M23 - M13*M22)),
        s*(M12*(M24*M33 - M23*M34) + M13*(M22*M34 - M24*M32) + M14*(M23*M32 - M22*M33)),
        s*(M23*(M31*M44 - M34*M41) + M24*(M33*M41 - M31*M43) + M21*(M34*M43 - M33*M44)),
        s*(M33*(M11*M44 - M14*M41) + M34*(M13*M41 - M11*M43) + M31*(M14*M43 - M13*M44)),
        s*(M43*(M11*M24 - M14*M21) + M44*(M13*M21 - M11*M23) + M41*(M14*M23 - M13*M24)),
        s*(M13*(M24*M31 - M21*M34) + M14*(M21*M33 - M23*M31) + M11*(M23*M34 - M24*M33)),
        s*(M24*(M31*M42 - M32*M41) + M21*(M32*M44 - M34*M42) + M22*(M34*M41 - M31*M44)),
        s*(M34*(M11*M42 - M12*M41) + M31*(M12*M44 - M14*M42) + M32*(M14*M41 - M11*M44)),
        s*(M44*(M11*M22 - M12*M21) + M41*(M12*M24 - M14*M22) + M42*(M14*M21 - M11*M24)),
        s*(M14*(M22*M31 - M21*M32) + M11*(M24*M32 - M22*M34) + M12*(M21*M34 - M24*M31)),
        s*(M21*(M33*M42 - M32*M43) + M22*(M31*M43 - M33*M41) + M23*(M32*M41 - M31*M42)),
        s*(M31*(M13*M42 - M12*M43) + M32*(M11*M43 - M13*M41) + M33*(M12*M41 - M11*M42)),
        s*(M41*(M13*M22 - M12*M23) + M42*(M11*M23 - M13*M21) + M43*(M12*M21 - M11*M22)),
        s*(M11*(M22*M33 - M23*M32) + M12*(M23*M31 - M21*M33) + M13*(M21*M32 - M22*M31)));
}


YY::Vec3f Mat4f::XComponent()
{
    return YY::Vec3f(M11, M12, M13);
}

YY::Vec3f Mat4f::YComponent()
{
    return YY::Vec3f(M21, M22, M23);
}

YY::Vec3f Mat4f::ZComponent()
{
    return YY::Vec3f(M31, M32, M33);
}


YY::Vec3f Mat4f::PosComponent()
{
    return YY::Vec3f(M41, M42, M43);
}

void Mat4f::Transpose()
{
#define n_swap(x,y) { float t=x; x=y; y=t; }
    n_swap(M12, M21);
    n_swap(M13, M31);
    n_swap(M14, M41);
    n_swap(M23, M32);
    n_swap(M24, M42);
    n_swap(M34, M43);
}



 Mat4f operator * (const Mat4f& m0, const Mat4f& m1) 
 {
     Mat4f M2(
         m0.M11*m1.M11 + m0.M21*m1.M12 + m0.M31*m1.M13 + m0.M41*m1.M14,
         m0.M12*m1.M11 + m0.M22*m1.M12 + m0.M32*m1.M13 + m0.M42*m1.M14,
         m0.M13*m1.M11 + m0.M23*m1.M12 + m0.M33*m1.M13 + m0.M43*m1.M14,
         m0.M14*m1.M11 + m0.M24*m1.M12 + m0.M34*m1.M13 + m0.M44*m1.M14,
 
         m0.M11*m1.M21 + m0.M21*m1.M22 + m0.M31*m1.M23 + m0.M41*m1.M24,
         m0.M12*m1.M21 + m0.M22*m1.M22 + m0.M32*m1.M23 + m0.M42*m1.M24,
         m0.M13*m1.M21 + m0.M23*m1.M22 + m0.M33*m1.M23 + m0.M43*m1.M24,
         m0.M14*m1.M21 + m0.M24*m1.M22 + m0.M34*m1.M23 + m0.M44*m1.M24,
 
         m0.M11*m1.M31 + m0.M21*m1.M32 + m0.M31*m1.M33 + m0.M41*m1.M34,
         m0.M12*m1.M31 + m0.M22*m1.M32 + m0.M32*m1.M33 + m0.M42*m1.M34,
         m0.M13*m1.M31 + m0.M23*m1.M32 + m0.M33*m1.M33 + m0.M43*m1.M34,
         m0.M14*m1.M31 + m0.M24*m1.M32 + m0.M34*m1.M33 + m0.M44*m1.M34,
 
         m0.M11*m1.M41 + m0.M21*m1.M42 + m0.M31*m1.M43 + m0.M41*m1.M44,
         m0.M12*m1.M41 + m0.M22*m1.M42 + m0.M32*m1.M43 + m0.M42*m1.M44,
         m0.M13*m1.M41 + m0.M23*m1.M42 + m0.M33*m1.M43 + m0.M43*m1.M44,
         m0.M14*m1.M41 + m0.M24*m1.M42 + m0.M34*m1.M43 + m0.M44*m1.M44);
     return M2;
 }


Vec3f operator * (const Mat4f& m, const Vec3f& v)
{
    return Vec3f(
        m.M11*v.x + m.M21*v.y + m.M31*v.z + m.M41,
        m.M12*v.x + m.M22*v.y + m.M32*v.z + m.M42,
        m.M13*v.x + m.M23*v.y + m.M33*v.z + m.M43);
}

//------------------------------------------------------------------------------
/**
*/
Vec4f operator * (const Mat4f& m, const Vec4f& v)
{
    return Vec4f(
        m.M11*v.x + m.M21*v.y + m.M31*v.z + m.M41*v.w,
        m.M12*v.x + m.M22*v.y + m.M32*v.z + m.M42*v.w,
        m.M13*v.x + m.M23*v.y + m.M33*v.z + m.M43*v.w,
        m.M14*v.x + m.M24*v.y + m.M34*v.z + m.M44*v.w);
};

NS_YY_END