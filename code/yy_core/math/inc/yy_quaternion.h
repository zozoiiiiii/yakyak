/************************************************************************/
/*
��Ԫ��
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

    //  slerp=spherical linear interpolation�������Բ�ֵ, ӳ�䵽3D��ת·����Ч���������Ź̶�����������ת
    // slerp(p,q,t) = (p*sin((1-t)*omega) + q*sin(t*omega)) / sin(omega)
    // tΪ��ֵϵ����t=0�����Ϊfrom; t=1,���Ϊto;
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
