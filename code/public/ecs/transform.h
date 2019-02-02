/************************************************************************/
/* 
@author:    junliang
@brief:     场景中的可视对象， 拥有位置，可以渲染。
@time:      12/9/2016
*/
/************************************************************************/
#pragma once


#include "math/inc/yy_vec3f.h"
#include "math/inc/yy_mat4f.h"
#include "core/inc/yy_transform.h"

NS_YY_BEGIN
class Transform
{
	YY_BEGIN(Transform, "ITransform");
    YY_FIELD(&Transform::m_pos, "position", "");
    YY_FIELD(&Transform::m_angle, "angle", "");
    YY_FIELD(&Transform::m_scale, "scale", "");
    YY_END
public:
    Transform();
    ~Transform();

    virtual YY::Vec3f GetPos(){return m_pos;}
    virtual void SetPos(const YY::Vec3f& val){m_pos = val;}
    virtual void AddPosX(float x);
    virtual void AddPosY(float y);
    virtual void AddPosZ(float z);

    virtual YY::Vec3f GetAngle(){return m_angle;}
    virtual void SetAngle(const YY::Vec3f& val){m_angle = val;}
    virtual YY::Vec3f GetScale(){return m_scale;}
    virtual void SetScale(const YY::Vec3f& val){m_scale = val;}

    virtual void GetRotateMatrix(YY::Mat4f* rotateM) const;
    virtual void GetTranslateMatrix(YY::Mat4f* pMatrix) const;
    virtual void GetScaleMatrix(YY::Mat4f* pMatrix) const;
    virtual void GetTMMatrix(YY::Mat4f* pMatrix) const;

    virtual void AddAngleX(float x);
    virtual void AddAngleY(float y);
    virtual void AddAngleZ(float z);

	virtual void SetParent(Transform* pTransform);
	virtual Transform* FindParent();
	virtual VarList GetChildren();
	virtual bool IsChildExist(Transform* pTransform);
	virtual void AddChild(Transform* pTransform);
	virtual void RemoveChild(Transform* pTransform);
private:
    YY::Vec3f m_pos;
    YY::Vec3f m_angle;
    YY::Vec3f m_scale;

	YY_OBJECTID m_parent;
	std::vector<YY_OBJECTID> m_children;
};

NS_YY_END

YY_TYPEHELPER(Transform, YVT_CLASS);