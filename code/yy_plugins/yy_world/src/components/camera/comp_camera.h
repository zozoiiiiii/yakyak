/************************************************************************/
/* 
@author:    junliang
@brief:     
@time:      11/29/2016
*/
/************************************************************************/
#pragma once

#include <string>
#include <vector>
#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"
#include "yy_world/inc/components/i_comp_camera.h"
#include "math/inc/yy_frustum.h"

class Comp_Camera : public IComp_Camera
{
    YY_BEGIN(Comp_Camera, IComp_Camera);
    YY_METHOD(&Comp_Camera::SetPersp, "SetPersp", "");
    YY_END
public:
    ~Comp_Camera(){}
    virtual void OnCreate(const VariantMap& args);
    virtual void OnDestroy();

    // perspective projection，透视投影
    virtual YY::Mat4f GetPersp();
    virtual void SetPersp(float fovy, float aspect, float nearZ, float farZ);


    //virtual YY::Mat4f GetProjection();
    //virtual void SetPerspective(float fovy, float aspect, float n, float f);
    //virtual void SetOrtho(float left, float right, float bottom, float top, float n, float f);

    // let the camera
    bool Bind(YY_OBJECTID id);

    void GetRayDirection(float mouseX, float mouseY, float& x, float& y, float& z, bool draw_ray=false);

    bool isOutOfFrustum(YY::AABB aabb);
private:
    void OnEvent_Resize(const char* name, const YY::VarList& args);
private:
    YY::Mat4f m_persp;    // 投影矩阵=
    YY::Frustum m_frustum;
    float m_fov;
    float m_nearZ;
    float m_farZ;
};