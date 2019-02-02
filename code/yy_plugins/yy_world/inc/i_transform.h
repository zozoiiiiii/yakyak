/************************************************************************/
/* 
@author:    junliang
@brief:     场景中的可视对象， 拥有位置，可以渲染。
@time:      12/9/2016
*/
/************************************************************************/
#pragma once


#include "yy_core.h"

class ITransform
{
public:
    virtual YY::Vec3f GetPos() = 0;
    virtual void SetPos(const YY::Vec3f& val) = 0;
    virtual void SetPos(float x, float y, float z){ SetPos(YY::Vec3f(x,y,z)); };
    virtual void AddPosX(float x) = 0;
    virtual void AddPosY(float y) = 0;
    virtual void AddPosZ(float z) = 0;
    virtual YY::Vec3f GetAngle() = 0;
    virtual void SetAngle(const YY::Vec3f& val) = 0;
    virtual void SetAngle(float x, float y, float z){ SetAngle(YY::Vec3f(x,y,z)); };

    // pitch
    virtual void AddAngleX(float x)=0;

    // yaw
    virtual void AddAngleY(float y)=0;

    // roll
    virtual void AddAngleZ(float z)=0;

    virtual YY::Vec3f GetScale() = 0;
    virtual void SetScale(const YY::Vec3f& val) = 0;
    virtual void SetScale(float x, float y, float z){ SetScale(YY::Vec3f(x,y,z)); };

    virtual void GetRotateMatrix(YY::Mat4f* rotateM) const = 0;
    virtual void GetTranslateMatrix(YY::Mat4f* pMatrix) const = 0;
    virtual void GetScaleMatrix(YY::Mat4f* pMatrix) const = 0;
    virtual void GetTMMatrix(YY::Mat4f* pMatrix) const = 0;
};