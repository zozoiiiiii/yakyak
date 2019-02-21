#include "weather.h"
#include "../scene/scene.h"
#include "yy_world/inc/i_world.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

void Weather::OnCreate(const VariantMap& args)
{
	Scene* pScene = (Scene*)IWorld::Instance()->GetScene();
    m_pRenderCxt = pScene->GetRenderCxt();

    //light
    {
        m_pLight_Ambient = new AmbientLight;
        m_pDirLight = new DirLight;

        // if only direction light, will cause the shadow black.  ambient will let the black shadow be seen
        m_pLight_Ambient->m_color = YY::Vec3f(1,1,1);    //white
        m_pLight_Ambient->m_intensity = 0.5f;

        m_pDirLight->color = YY::Vec3f(1,1,1); // white color
        m_pDirLight->direction = YY::Vec3f(0.0f, -1.0f, -1.0f);
        m_pDirLight->intensity = 0.5f;
    }

    Apply();
}


void Weather::Apply()
{
    // ambient
    m_pRenderCxt->ambient_color = YY::Vec3f(m_pLight_Ambient->m_color.x, m_pLight_Ambient->m_color.y, m_pLight_Ambient->m_color.z);
    m_pRenderCxt->ambient_intensity = m_pLight_Ambient->m_intensity;

    // direction light
    m_pRenderCxt->directionLight_color = YY::Vec3f(m_pDirLight->color.x, m_pDirLight->color.y, m_pDirLight->color.z);
    m_pRenderCxt->directionLight_intensity = m_pDirLight->intensity;
    m_pRenderCxt->directionLight_direction = YY::Vec3f(m_pDirLight->direction.x, m_pDirLight->direction.y, m_pDirLight->direction.z);

}