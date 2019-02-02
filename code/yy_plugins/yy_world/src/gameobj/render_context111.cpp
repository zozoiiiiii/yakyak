#include "render_context.h"
#include "render/inc/i_shader.h"
#include "world/inc/i_world.h"
#include "world/inc/i_event_mgr.h"

void RenderContext::OnCreate()
{
    m_render_type = RT_Normal;
}

void RenderContext::OnDestroy()
{
    
}

void RenderContext::SetViewMatrixWithOutPos(const ITransform* pTM)
{
    YY::Mat4f rotate, scale;
    pTM->GetRotateMatrix(&rotate);
    pTM->GetScaleMatrix(&scale);
    m_viewM = rotate * scale;
    m_viewM.Inverse();
    return;
}

void RenderContext::SetViewMatrix(const ITransform* pTM)
{
    m_viewM.Ident();
    pTM->GetTMMatrix(&m_viewM);
    m_viewM.Inverse();
    return;
}


void RenderContext::SetViewMatrix(YY::Mat4f m)
{
    m_viewM = m;
}

void RenderContext::SetProjMatrix(YY::Mat4f m)
{
    m_projM = m;
}

void RenderContext::SetResPath(const std::string& path)
{
    m_res_path = path;
    GetMgr()->SetGlobal("ResourcePath", m_res_path);
}

void RenderContext::SetWinSize(int w, int h){
    if(m_nWinWidth == w && m_nWinHeight == h)
        return;

    m_nWinWidth = w;
    m_nWinHeight = h;
    GetMgr()->SetGlobal("Width", m_nWinWidth);
    GetMgr()->SetGlobal("Height", m_nWinHeight);
    IEventMgr* pEventMgr = IWorld::Instance()->GetEventMgr();
    pEventMgr->Invoke(GetID(), "OnEvent_RenderWindow_Resize", YY::VarList()<<m_nWinWidth<<m_nWinHeight);

}

void RenderContext::SetDeviceSize(int w, int h, bool bWindowSizeChange){
    if(m_nDeviceWidth == w && m_nDeviceHeight == h)
        return;

    m_nDeviceWidth = w;
    m_nDeviceHeight = h;
    GetMgr()->SetGlobal("Width", m_nDeviceWidth);
    GetMgr()->SetGlobal("Height", m_nDeviceHeight);

    IEventMgr* pEventMgr = IWorld::Instance()->GetEventMgr();
    pEventMgr->Invoke(GetID(), "OnEvent_Device_Resize", YY::VarList()<<m_nDeviceWidth<<m_nDeviceHeight<<bWindowSizeChange);
}
