#include "comp_camera.h"
#include "yy_world/inc/i_world.h"
#include "yy_world/inc/i_scene.h"
//#include "../primitive/lines.h"
//#include "yy_world/inc/i_event_mgr.h"

void Comp_Camera::OnCreate()
{
    IEventMgr* pEventMgr = IWorld::Instance()->GetEventMgr();
    pEventMgr->ConnectGlobals("OnEvent_RenderWindow_Resize",  fastdelegate::MakeDelegate(this, &Comp_Camera::OnEvent_Resize));
}

void Comp_Camera::OnDestroy()
{
}

YY::Mat4f Comp_Camera::GetPersp()
{
    return m_persp;
}

void Comp_Camera::SetPersp(float fovy, float aspect, float nearZ, float farZ)
{
    m_fov = fovy;
    m_nearZ = nearZ;
    m_farZ = farZ;

    m_persp.PerspFovRh(fovy, aspect, nearZ, farZ);
}


bool Comp_Camera::Bind(YY_OBJECTID id)
{
    return false;
}

void Comp_Camera::OnEvent_Resize(const char* name, const YY::VarList& args)
{
    int w = args.GetInt(0);
    int h = args.GetInt(1);

    float aspect = (float)w/(float)h;
    SetPersp(m_fov, aspect, m_nearZ, m_farZ);
}

void Comp_Camera::GetRayDirection(float mouseX, float mouseY, float& x, float& y, float& z, bool draw_ray)
{
    IScene* pScene = IWorld::Instance()->GetScene();
    RenderContext* pRenderCxt = pScene->GetRenderCxt();

    const YY::Mat4f& mtxProj = pRenderCxt->projMatrix;
    YY::Mat4f proj_inverse = mtxProj;
    proj_inverse.Inverse();

    const YY::Mat4f& view = pRenderCxt->viewMatrix;
    YY::Mat4f view_inverse = view;
    view_inverse.Inverse();

    float width = pRenderCxt->w;
    float height = pRenderCxt->h;

    /* screen space -> ndc space
    xndc = (xw-x)/(width/2)-1
    yndc = (yw-y)/(height/2)-1

    npc space to screen space:
    Xw = (xndc+1)*(width/2)+x
    yw = (yndc+1)*(height/2)+y
    where x,y,width,height are the parameters to glViewport().
    */
    float x0 = (2.0f * mouseX) / width - 1.0f;  
    float y0 = 1.0f - (2.0f * mouseY) / height;  
    float z0 = 1.0f; 
    YY::Vec3f ray_nds = YY::Vec3f (x0, y0, z0);

    // ndc space -> clip space
    YY::Vec4f ray_clip;
    ray_clip.x = ray_nds.x;
    ray_clip.y = ray_nds.y;
    ray_clip.z = ray_nds.z;
    ray_clip.w = 1.0f;

    // clip space -> eye space
    YY::Vec4f ray_eye = proj_inverse * ray_clip;

    // eye space -> world space
    YY::Vec4f ray_world = view_inverse * ray_eye;  

    if (ray_world.w != 0.0)  
    {  
        ray_world.x /= ray_world.w;  
        ray_world.y /= ray_world.w;  
        ray_world.z /= ray_world.w;
    }  

    YY::Vec3f ray_orig;
    ray_orig.x = view_inverse.M41;
    ray_orig.y = view_inverse.M42;
    ray_orig.z = view_inverse.M43;

    YY::Vec3f ray_dir;
    ray_dir.x = ray_world.x - ray_orig.x;
    ray_dir.y = ray_world.y - ray_orig.y;
    ray_dir.z = ray_world.z - ray_orig.z;
    ray_dir.norm();
    ray_dir = ray_dir * 1000.0f;

    x=ray_dir.x;
    y=ray_dir.y;
    z=ray_dir.z;

    // draw ray line.
    if(draw_ray)
    {
//         static IGameObj* pTestLine=NULL;
//         Lines* pComp_Lines=NULL;
//         if(NULL == pTestLine)
//         {
//             IScene* pScene = IWorld::Instance()->GetScene();
//             pTestLine = pScene->CreateObj();
// 			pTestLine->SetName("dray_ray_line");
//             pScene->AddObj(pTestLine);
//             pComp_Lines = (Lines*)pTestLine->AddComp("Lines");
//         }
// 
//         pComp_Lines = (Lines*)pTestLine->GetComp("Lines");
//         YY::Vec3f ray_dst(ray_world.x, ray_world.y, ray_world.z);
//         pComp_Lines->DrawSingleLine(ray_orig, ray_dst, 0xFF00FF00);
    }
}


bool Comp_Camera::isOutOfFrustum(YY::AABB aabb)
{
    // »ñµÃfrustum.
    YY::Mat4f view;
	IGameObj* pGameObj = FindGameObj();
	if (!pGameObj)
		return false;

    pGameObj->GetTransform()->GetTMMatrix(&view);
    view.Inverse();

    YY::Mat4f proj = GetPersp();
    YY::Mat4f vp = proj * view;
    m_frustum.initFrustumFromProjectMatrix(vp);

    return m_frustum.isOutOfFrustum(aabb);
}