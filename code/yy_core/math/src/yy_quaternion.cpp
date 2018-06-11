#include "../inc/yy_Quaternion.h"

#include <math.h>

#define delta 1e-10     // error tolerance

NS_YY_BEGIN
Quaternion::Quaternion()
: w(1), x(0), y(0), z(0)
{
}

Quaternion::Quaternion(float w, float x, float y, float z)
: w(w), x(x), y(y), z(z)
{
}

void Quaternion::fromAxisAngle(float angle, float x, float y, float z)
{
    float half_angle = angle / 2.f;

    // angle to radius
    float half = YY_PI * half_angle /180.0f;
    float sinHalf = sinf(half);
    w = cosf(half);
    x = sinHalf * cosf(x);
    y = sinHalf * cosf(y);
    z = sinHalf * cosf(z);
}

void Quaternion::set(float argw, float argx, float argy, float argz)
{
    w=argw;
    x=argx;
    y=argy;
    z=argz;
}

Quaternion::Quaternion(float roll, float pitch, float yaw)
{
    float cr = cos(roll/2);
    float cp = cos(pitch/2);
    float cy = cos(yaw/2);

    float sr = sin(roll/2);
    float sp = sin(pitch/2);
    float sy = sin(yaw/2);

    float cpcy = cp * cy;
    float spsy = sp * sy;
    float cpsy = cp * sy;
    float spcy = sp * cy;

    w = cr * cpcy + sr * spsy;
    x = sr * cpcy - cr * spsy;
    y = cr * spcy + sr * cpsy;
    z = cr * cpsy - sr * spcy;
}

Quaternion &Quaternion::operator*=(const Quaternion &q)
{
    float e, f, g, h;

    float tw = w;
    float tx = x;
    float ty = y;

//        a = (q1->w + q1->x)*(q2->w + q2->x);
//        b = (q1->z - q1->y)*(q2->y - q2->z);
//        c = (q1->w - q1->x)*(q2->y + q2->z); 
//        d = (q1->y + q1->z)*(q2->w - q2->x);
    e = (tx +  z)*(q.x + q.y);
    f = (tx -  z)*(q.x - q.y);
    g = (tw + ty)*(q.w - q.z);
    h = (tw - ty)*(q.w + q.z);

    w = /*b*/( z - ty)*(q.y - q.z) + (-e - f + g + h)/2;
    x = /*a*/(tw + tx)*(q.w + q.x) - ( e + f + g + h)/2; 
    y = /*c*/(tw - tx)*(q.y + q.z) + ( e - f + g - h)/2; 
    z = /*d*/(ty +  z)*(q.w - q.x) + ( e - f - g + h)/2;

//        w = tw*q.w - tx*q.x - ty*q.y - tz*q.z;
//        x = tw*q.x + tx*q.w + ty*q.z - tz*q.y;
//        y = tw*q.y + ty*q.w + tz*q.x - tx*q.z;
//        z = tw*q.z + tz*q.w + tx*q.y - ty*q.x;
    return *this;
}

Vec3f operator* (const Quaternion& quat, const Vec3f& vec)
{
    double x = vec.x, y = vec.y, z = vec.z,
        qx = quat.x, qy = quat.y, qz = quat.z, qw = quat.w,

        // calculate quat * vec
        ix = qw * x + qy * z - qz * y,
        iy = qw * y + qz * x - qx * z,
        iz = qw * z + qx * y - qy * x,
        iw = -qx * x - qy * y - qz * z;

    Vec3f dest;

    // calculate result * inverse quat
    dest.x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    dest.y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    dest.z = iz * qw + iw * -qz + ix * -qy - iy * -qx;

    return dest;
}

Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
    Quaternion qr;

    float e, f, g, h;

//        a = (q1->w + q1->x)*(q2->w + q2->x);
//        b = (q1->z - q1->y)*(q2->y - q2->z);
//        c = (q1->w - q1->x)*(q2->y + q2->z); 
//        d = (q1->y + q1->z)*(q2->w - q2->x);
    e = (q1.x + q1.z)*(q2.x + q2.y);
    f = (q1.x - q1.z)*(q2.x - q2.y);
    g = (q1.w + q1.y)*(q2.w - q2.z);
    h = (q1.w - q1.y)*(q2.w + q2.z);

    qr.w = /*b*/(q1.z - q1.y)*(q2.y - q2.z) + (-e - f + g + h)/2;
    qr.x = /*a*/(q1.w + q1.x)*(q2.w + q2.x) - ( e + f + g + h)/2; 
    qr.y = /*c*/(q1.w - q1.x)*(q2.y + q2.z) + ( e - f + g - h)/2; 
    qr.z = /*d*/(q1.y + q1.z)*(q2.w - q2.x) + ( e - f - g + h)/2;

//        qr.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
//        qr.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
//        qr.y = q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z;
//        qr.z = q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x;

    return qr;
}
/*
quatmul(quat *q1, quat *q2, quat *res){


float a, b, c, d, e, f, g, h;


a = (q1->w + q1->x)*(q2->w + q2->x);
b = (q1->z - q1->y)*(q2->y - q2->z);
c = (q1->w - q1->x)*(q2->y + q2->z); 
d = (q1->y + q1->z)*(q2->w - q2->x);
e = (q1->x + q1->z)*(q2->x + q2->y);
f = (q1->x - q1->z)*(q2->x - q2->y);
g = (q1->w + q1->y)*(q2->w - q2->z);
h = (q1->w - q1->y)*(q2->w + q2->z);


res->w = b + (-e - f + g + h) /2;
res->x = a - (e + f + g + h)/2; 
res->y = c + (e - f + g - h)/2; 
res->z = d + (e - f - g + h)/2;
}
*/

// smoothly (spherically, shortest path on a Quaternion sphere) 
// interpolates between two unit Quaternion positions

// most of this code is optimized for speed and not for readability
// slerp(p,q,t) = (p*sin((1-t)*omega) + q*sin(t*omega)) / sin(omega)
void Quaternion::slerp(const Quaternion &to, float t)
{
    float to1[4];
    float omega, cosom, sinom;
    float scale0, scale1;

    // calc cosine
    cosom = x*to.x + y*to.y + z*to.z + w*to.w;

    // adjust signs (if necessary)
    if (cosom < 0.0)
    {
        cosom = -cosom;
        to1[0] = - to.x;
        to1[1] = - to.y;
        to1[2] = - to.z;
        to1[3] = - to.w;
    } else  {
        to1[0] = to.x;
        to1[1] = to.y;
        to1[2] = to.z;
        to1[3] = to.w;
    }

    // calculate coefficients
    if ((1.0 - cosom) > delta) 
    {
        // standard case (slerp)
        omega = acos(cosom);
        sinom = sin(omega);
        scale0 = (float)sin((1.0 - t) * omega) / sinom;
        scale1 = (float)sin(t * omega) / sinom;
    } else {        
        // "from" and "to" Quaternions are very close 
        //  ... so we can do a linear interpolation
        scale0 = 1.0f - t;
        scale1 = t;
    }

    // calculate final values
    x = scale0*x + scale1*to1[0];
    y = scale0*y + scale1*to1[1];
    z = scale0*z + scale1*to1[2];
    w = scale0*w + scale1*to1[3];
}

void Quaternion::slerp(const Quaternion &from, const Quaternion &to, float t)
{
    float to1[4];
    float omega, cosom, sinom;
    float scale0, scale1;

    // calc cosine
    cosom = from.x*to.x + from.y*to.y + from.z*to.z + from.w*to.w;

    // adjust signs (if necessary)
    if (cosom < 0.0)
    {
        cosom = -cosom;
        to1[0] = - to.x;
        to1[1] = - to.y;
        to1[2] = - to.z;
        to1[3] = - to.w;
    } else  {
        to1[0] = to.x;
        to1[1] = to.y;
        to1[2] = to.z;
        to1[3] = to.w;
    }

    // calculate coefficients
    if ((1.0 - cosom) > delta) 
    {
        // standard case (slerp)
        omega = acos(cosom);
        sinom = sin(omega);
        scale0 = (float)sin((1.0 - t) * omega) / sinom;
        scale1 = (float)sin(t * omega) / sinom;
    } else {        
        // "from" and "to" Quaternions are very close 
        //  ... so we can do a linear interpolation
        scale0 = 1.0f - t;
        scale1 = t;
    }

    // calculate final values
    x = scale0*from.x + scale1*to1[0];
    y = scale0*from.y + scale1*to1[1];
    z = scale0*from.z + scale1*to1[2];
    w = scale0*from.w + scale1*to1[3];
}


// linearly interpolates between two Quaternion positions
// fast but not as nearly as smooth as slerp
void Quaternion::lerp(const Quaternion &to, float t)
{
    float to1[4];
    float cosom;
    float scale0, scale1;

    // calc cosine
    cosom = x*to.x + y*to.y + z*to.z + w*to.w;

    // adjust signs (if necessary)
    if ( cosom < 0.0 )
    {
        to1[0] = - to.x;
        to1[1] = - to.y;
        to1[2] = - to.z;
        to1[3] = - to.w;
    } else  {
        to1[0] = to.x;
        to1[1] = to.y;
        to1[2] = to.z;
        to1[3] = to.w;
    }

    // interpolate linearly
    scale0 = 1.0f - t;
    scale1 = t;

    // calculate final values
    x = scale0*x + scale1*to1[0];
    y = scale0*y + scale1*to1[1];
    z = scale0*z + scale1*to1[2];
    w = scale0*w + scale1*to1[3];
}


Quaternion Quaternion::normalize()
{
    double len = sqrt(x * x + y * y + z * z + w * w);
    if (len == 0) {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
        return *this;
    }
    len = 1 / len;
    x = x * len;
    y = y * len;
    z = z * len;
    w = w * len;

    return *this;
}
NS_YY_END