/************************************************************************/
/*
四元数
*/
/************************************************************************/
#ifndef Quaternion_h
#define Quaternion_h
#include "reflection/yy_type.h"
#include "../inc/yy_Vec3f.h"

NS_YY_BEGIN

class Quaternion
{
public:
    float w, x, y, z;

    Quaternion();
    Quaternion(float w, float x, float y, float z);

    // from auler angle
    Quaternion(float rotx, float roty, float rotz);

    // from axis angle
    void fromAxisAngle(float angle, float x, float y, float z);



    void set(float w, float x, float y, float z);

    //  slerp=spherical linear interpolation球面线性插值, 映射到3D旋转路径后，效果就是绕着固定轴做匀速旋转
    // slerp(p,q,t) = (p*sin((1-t)*omega) + q*sin(t*omega)) / sin(omega)
    // t为插值系数，t=0，结果为from; t=1,结果为to;
    void slerp(const Quaternion &to, float t);
    void slerp(const Quaternion &from, const Quaternion &to, float t);
    void lerp(const Quaternion &to, float t);

//        float magnitude();
//        Quaternion inverse();

    Quaternion normalize();

       Quaternion &operator*=(const Quaternion &q);
    friend Quaternion operator*(const Quaternion &q1, const Quaternion &q2);
    friend Vec3f operator*(const Quaternion &q1, const Vec3f &v2);
};

NS_YY_END

#endif //Quaternion_h
