/************************************************************************/
/* 
@author:    junliang
@brief:     transform component
@time:      12/9/2016
*/
/************************************************************************/
#pragma once

#include "i_entity.h"
#include "math/inc/yy_mat4f.h"
#include "math/inc/yy_vec3f.h"

NS_YY_BEGIN
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


	virtual void SetParent(ITransform* pTransform) = 0;
	virtual ITransform* FindParent() = 0;
	virtual VarList GetChildren() = 0;
	virtual bool IsChildExist(ITransform* pTransform) = 0;
	virtual void AddChild(ITransform* pTransform) = 0;
	virtual void RemoveChild(ITransform* pTransform) = 0;
};

NS_YY_END