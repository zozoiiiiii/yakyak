/************************************************************************/
/* 
@author:    junliang
@brief:     weather: 
light, fog
@time:      2017-6-16
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_render/inc/i_render.h"

struct AmbientLight
{
    YY::Vec3f m_color;
    float m_intensity;
};


struct DirLight
{
    YY::Vec3f color;
    YY::Vec3f direction;
    float intensity;
};


class Weather : public YY::BaseObject
{
    YY_BEGIN(Weather, "BaseObject");
    YY_END
public:
    virtual void OnCreate();
    virtual void OnDestroy(){}
    DirLight* GetDirLight(){return m_pDirLight;}
    AmbientLight* GetAmbientLight(){return m_pLight_Ambient;}
    void Apply();

private:
    RenderContext* m_pRenderCxt;

    DirLight* m_pDirLight;              // sun
    AmbientLight* m_pLight_Ambient;
};