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
#include "../i_game_obj.h"
#include "math/inc/yy_aabb.h"

class IComp_Camera : public GameObjComponent
{
public:
    // perspective projection，透视投影
    virtual YY::Mat4f GetPersp() = 0;
    virtual void SetPersp(float fovy, float aspect, float nearZ, float farZ) = 0;
    virtual void GetRayDirection(float mouseX, float mouseY, float& x, float& y, float& z, bool draw_ray=false) = 0;
    virtual bool isOutOfFrustum(YY::AABB aabb) = 0;

    // 存放摄像机渲染结果的rt
    // virtual void SetRT(IColorTexture* pRT) = 0;
};