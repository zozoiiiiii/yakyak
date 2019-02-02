#include "edit_axis.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "yy_world/inc/i_world.h"
#include "yy_world/inc/i_scene.h"
#include "yy_world/inc/i_render_context.h"
#include "lines.h"

// 0xFFFF0000 red
// 0xFF00FF00 green
// 0xFF0000FF blue

void EditAxis::OnCreate()
{
    glGenBuffers(1, &m_vbo_line);
    glGenBuffers(1, &m_vbo_pick);

    m_bShowPlaneXY = false;
    m_bShowPlaneXZ = false;
    m_bShowPlaneYZ = false;
    m_bSelectX = false;
    m_bSelectY = false;
    m_bSelectZ = false;

    //InitVertex();
}

void EditAxis::InitVertex()
{
    YY::Vec3f center(0.0f, 0.0f, 0.0f);

    YY::Vec3f pos = GetOwner()->GetTransform()->GetPos();
    RenderContext* pRenderCxt = GetOwner()->GetScene()->GetRenderCxt();
    YY::Vec3f pos_in_view =  pRenderCxt->GetViewMatrix() * pos;

    float distance = YY::Distance(0,0,0,pos_in_view.x, pos_in_view.y, pos_in_view.z);
    YY::Vec3f v_target = pos_in_view.norm();
    YY::Vec3f v_camera_z = YY::Vec3f(0,0,-1);
    float dot = v_target.dot(v_camera_z);
    float distance_c = distance * cos(dot);

    float fAxisLength = 1.0f*2;
    float len = fAxisLength * distance_c * 0.1;
    len = max(len, 2.0f);

    YY::Vec3f x_axis(len, 0.0f, 0.0f); 
    YY::Vec3f y_axis(0.0f, len, 0.0f); 
    YY::Vec3f z_axis(0.0f, 0.0f, len); 

    unsigned int x_color = 0xFFFF0000;
    unsigned int y_color = 0xFF00FF00;
    unsigned int z_color = 0xFF0000FF;

    unsigned int xy_color = 0x80FFFF00;
    unsigned int xz_color = 0x80FF00FF;
    unsigned int yz_color = 0x8000FFFF;

    unsigned int select_color = 0x80FFFFFF;

    if (m_bSelectX && m_bSelectY)
    {
        xy_color = select_color;
    }

    if (m_bSelectX && m_bSelectZ)
    {
        xz_color = select_color;
    }

    if (m_bSelectY && m_bSelectZ)
    {
        yz_color = select_color;
    }

    if (m_bSelectX)
    {
        x_color = select_color;
    }

    if (m_bSelectY)
    {
        y_color = select_color;
    }

    if (m_bSelectZ)
    {
        z_color = select_color;
    }

    // 箭杆
    vertex_line_t* lines = m_LineVertex;

    lines[0].pos = center;
    lines[1].pos = x_axis;
    lines[2].pos = center;
    lines[3].pos = y_axis;
    lines[4].pos = center;
    lines[5].pos = z_axis;

    lines[0].diffuse = x_color;
    lines[1].diffuse = x_color;
    lines[2].diffuse = y_color;
    lines[3].diffuse = y_color;
    lines[4].diffuse = z_color;
    lines[5].diffuse = z_color;

    m_nPickTriangleNum = 0;

    // 箭头
    vertex_line_t* arrows = m_PickVertex;


    float fArrowWidth = 0.1*3;
    float fArrowHeight = 0.2*3;
    float d1 = fArrowHeight;
    float d2 = fArrowWidth * 0.5f;
    float d3 = fArrowWidth * 0.3f;

    YY::Vec3f x_v1(len - d1, d2, 0.0f);
    YY::Vec3f x_v2(len - d1, -d3, d3);
    YY::Vec3f x_v3(len - d1, -d3, -d3);

    // 一个箭头四个面，12个顶点
    arrows[0].pos = x_axis;
    arrows[1].pos = x_v1;
    arrows[2].pos = x_v2;
    arrows[3].pos = x_axis;
    arrows[4].pos = x_v2;
    arrows[5].pos = x_v3;
    arrows[6].pos = x_axis;
    arrows[7].pos = x_v3;
    arrows[8].pos = x_v1;
    arrows[9].pos = x_v1;
    arrows[10].pos = x_v2;
    arrows[11].pos = x_v3;

    for (size_t x = 0; x < 12; ++x)
    {
        arrows[x].diffuse = x_color;
    }

    // 记录这4个面对应的轴
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_X;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_X;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_X;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_X;

    arrows += 12;

    YY::Vec3f y_v1(d2, len - d1, 0.0f);
    YY::Vec3f y_v2(-d3, len - d1, d3);
    YY::Vec3f y_v3(-d3, len - d1, -d3);

    arrows[0].pos = y_axis;
    arrows[1].pos = y_v1;
    arrows[2].pos = y_v2;
    arrows[3].pos = y_axis;
    arrows[4].pos = y_v2;
    arrows[5].pos = y_v3;
    arrows[6].pos = y_axis;
    arrows[7].pos = y_v3;
    arrows[8].pos = y_v1;
    arrows[9].pos = y_v1;
    arrows[10].pos = y_v2;
    arrows[11].pos = y_v3;

    for (size_t y = 0; y < 12; ++y)
    {
        arrows[y].diffuse = y_color;
    }

    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Y;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Y;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Y;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Y;

    arrows += 12;

    YY::Vec3f z_v1(d2, 0.0f, len - d1);
    YY::Vec3f z_v2(-d3, d3, len - d1);
    YY::Vec3f z_v3(-d3, -d3, len - d1);

    arrows[0].pos = z_axis;
    arrows[1].pos = z_v1;
    arrows[2].pos = z_v2;
    arrows[3].pos = z_axis;
    arrows[4].pos = z_v2;
    arrows[5].pos = z_v3;
    arrows[6].pos = z_axis;
    arrows[7].pos = z_v3;
    arrows[8].pos = z_v1;
    arrows[9].pos = z_v1;
    arrows[10].pos = z_v2;
    arrows[11].pos = z_v3;

    for (size_t z = 0; z < 12; ++z)
    {
        arrows[z].diffuse = z_color;
    }

    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Z;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Z;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Z;
    m_nPickFlags[m_nPickTriangleNum++] = SELECT_Z;

    arrows += 12;

    // 双向平面
    vertex_line_t* planes = arrows;

    float half = (fAxisLength - fArrowHeight) * 0.5f;

    YY::Vec3f x_half(half, 0.0f, 0.0f);
    YY::Vec3f y_half(0.0f, half, 0.0f);
    YY::Vec3f z_half(0.0f, 0.0f, half);
    YY::Vec3f xy_half(half, half, 0.0f);
    YY::Vec3f xz_half(half, 0.0f, half);
    YY::Vec3f yz_half(0.0f, half, half);

    if (m_bShowPlaneXY)
    {
        planes[0].pos = center;
        planes[1].pos = x_half;
        planes[2].pos = y_half;
        planes[3].pos = x_half;
        planes[4].pos = y_half;
        planes[5].pos = xy_half;

        for (size_t x = 0; x < 6; ++x)
        {
            planes[x].diffuse = xy_color;
        }

        m_nPickFlags[m_nPickTriangleNum++] = SELECT_X + SELECT_Y;
        m_nPickFlags[m_nPickTriangleNum++] = SELECT_X + SELECT_Y;

        planes += 6;
    }

    if (m_bShowPlaneXZ)
    {
        planes[0].pos = center;
        planes[1].pos = x_half;
        planes[2].pos = z_half;
        planes[3].pos = x_half;
        planes[4].pos = z_half;
        planes[5].pos = xz_half;

        for (size_t y = 0; y < 6; ++y)
        {
            planes[y].diffuse = xz_color;
        }

        m_nPickFlags[m_nPickTriangleNum++] = SELECT_X + SELECT_Z;
        m_nPickFlags[m_nPickTriangleNum++] = SELECT_X + SELECT_Z;

        planes += 6;
    }

    if (m_bShowPlaneYZ)
    {
        planes[0].pos = center;
        planes[1].pos = y_half;
        planes[2].pos = z_half;
        planes[3].pos = y_half;
        planes[4].pos = z_half;
        planes[5].pos = yz_half;

        for (size_t z = 0; z < 6; ++z)
        {
            planes[z].diffuse = yz_color;
        }

        m_nPickFlags[m_nPickTriangleNum++] = SELECT_Y + SELECT_Z;
        m_nPickFlags[m_nPickTriangleNum++] = SELECT_Y + SELECT_Z;

        planes += 6;
    }


    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_line);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(vertex_line_t), &m_LineVertex[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pick);
    glBufferData(GL_ARRAY_BUFFER, 18 * 3*sizeof(vertex_line_t), &m_PickVertex[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void EditAxis::OnDestroy()
{
    
}

void EditAxis::OnAddRender(IBatchMgr* pBatchMgr)
{
    // 修改渲染优先级，渲染时取消深度测试，保证在模型的前面显示
    pBatchMgr->AddBatch(RP_TopMost, this);
}

void EditAxis::OnRender(RenderContext* pCxt)
{
    InitVertex();
    DrawAxisLine(pCxt);
    DrawAxisPick(pCxt);
}


void EditAxis::DrawAxisLine(RenderContext* pCxt)
{
    if(pCxt->GetRenderType() != RT_Normal)
        return;


    static IShader* pShader = NULL;
    if(NULL == pShader)
    {
        pShader = GetOwner()->GetRender()->CreateShader();
        std::string vsh = IWorld::Instance()->GetResPath() + "shader\\edit_line.vsh";
        std::string fsh = IWorld::Instance()->GetResPath() + "shader\\edit_line.fsh";
        pShader->Load(vsh.c_str(), fsh.c_str());
    }


    //告诉gpu一次要执行的命令
    pShader->Begin();

    YY::Mat4f model;
    GetOwner()->GetTransform()->GetTMMatrix(&model);

    YY::Mat4f viewM;
    viewM = pCxt->GetViewMatrix();

    YY::Mat4f mvp = pCxt->GetProjMatrix() * viewM * model;
    pShader->SetUniformMat4fv("c_mtxWVP", 1, mvp.m_data);

    // 激活vbo对象
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_line);
    int vertexLocation = glGetAttribLocation (pShader->GetProgramHandle(),"iPos");
    if(vertexLocation>=0)
    {
        glEnableVertexAttribArray (vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_line_t), (void*)0);
    }

    int diffuse_loc = glGetAttribLocation (pShader->GetProgramHandle(),"iDiffuse");
    if(diffuse_loc>=0)
    {
        glEnableVertexAttribArray(diffuse_loc);
        glVertexAttribPointer(diffuse_loc, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(vertex_line_t), (void*) (3 * sizeof(float)));
    }

    glDrawArrays(GL_LINES, 0, 6);
    glLineWidth(1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // 告诉gpu结束该命令
    pShader->End();
}

void EditAxis::DrawAxisPick(RenderContext* pCxt)
{
    if(pCxt->GetRenderType() != RT_Normal)
        return;


    static IShader* pShader = NULL;
    if(NULL == pShader)
    {
        pShader = GetOwner()->GetRender()->CreateShader();
        std::string vsh = IWorld::Instance()->GetResPath() + "shader\\edit_line.vsh";
        std::string fsh = IWorld::Instance()->GetResPath() + "shader\\edit_line.fsh";
        pShader->Load(vsh.c_str(), fsh.c_str());
    }


    //告诉gpu一次要执行的命令
    pShader->Begin();

    YY::Mat4f model;
    GetOwner()->GetTransform()->GetTMMatrix(&model);

    YY::Mat4f viewM;
    viewM = pCxt->GetViewMatrix();
    //viewM.SetTranslate(0,-2,-10);
    YY::Mat4f projM;
    projM = pCxt->GetProjMatrix();

    YY::Mat4f mvp = projM * viewM * model;
    pShader->SetUniformMat4fv("c_mtxWVP", 1, mvp.m_data);

    // 激活vbo对象
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pick);
    int vertexLocation = glGetAttribLocation (pShader->GetProgramHandle(),"iPos");
    if(vertexLocation>=0)
    {
        glEnableVertexAttribArray (vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_line_t), (void*)0);
    }

    int diffuse_loc = glGetAttribLocation (pShader->GetProgramHandle(),"iDiffuse");
    if(diffuse_loc>=0)
    {
        glEnableVertexAttribArray(diffuse_loc);
        glVertexAttribPointer(diffuse_loc, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(vertex_line_t), (void*) (3 * sizeof(float)));
    }

    glDrawArrays(GL_TRIANGLES, 0, 18*3);
    //glLineWidth(1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // 告诉gpu结束该命令
    pShader->End();
}




int FmIntersectTriEx(const YY::Vec3f* pv1,const YY::Vec3f* pv2,const YY::Vec3f* pv3,const YY::Vec3f* pRayPos,
                     const YY::Vec3f* pRayDir,float* pU,float* pV,float* pDist)
{
    // Find vectors for two edges sharing vert0  
    YY::Vec3f edge1 = *pv2 - *pv1;   
    YY::Vec3f edge2 = *pv3 - *pv1;    

    // Begin calculating determinant - also used to calculate U parameter   
    YY::Vec3f pvec;   

    //YY::Vec3fCross(&pvec, pRayDir, &edge2); 
    pvec = pRayDir->cross(edge2);

    // If determinant is near zero, ray lies in plane of triangle  
    //float det = YY::Vec3fDot(&edge1, &pvec);   
    float det = edge1.dot(pvec);
    YY::Vec3f tvec; 

    if (det > 0)   
    {    
        tvec = *pRayPos - *pv1;   
    }  
    else   
    {      
        tvec = *pv1 - *pRayPos;  
        det = -det; 
    }

    if (det < 0.0001F) 
    {
        return false; 
    }

    // Calculate U parameter and test bounds  
    //*pU = YY::Vec3fDot(&tvec, &pvec);  
    *pU = tvec.dot(pvec);

    if ((*pU < 0.0F) || (*pU > det))
    {
        return false;  
    }

    // Prepare to test V parameter    
    YY::Vec3f qvec; 

    //YY::Vec3fCross(&qvec, &tvec, &edge1); 
    qvec = tvec.cross(edge1);

    // Calculate V parameter and test bounds  
    //*pV = YY::Vec3fDot(pRayDir, &qvec); 
    *pV = pRayDir->dot(qvec); 

    if ((*pV < 0.0F) || ((*pU + *pV) > det))  
    {
        return false;   
    }

    // Calculate t, scale parameters, ray intersects triangle  
    //*pDist = YY::Vec3fDot(&edge2, &qvec);  
    *pDist = edge2.dot(qvec);  

    float fInvDet = 1.0F / det;  

    *pDist *= fInvDet;  
    *pU *= fInvDet;   
    *pV *= fInvDet;   

    return true;
}


YY::Vec3f* FmVec3TransformCoord(YY::Vec3f* pOut,const YY::Vec3f* pv,const YY::Mat4f* pm)
{
    //Assert( (pOut != NULL) && (pv != NULL) && (pm != NULL) );
    YY::Vec3f out;

    float norm;
    norm = pm->M14*pv->x + pm->M24*pv->y + pm->M34*pv->z + pm->M44;

    if(norm)
    {
        out.x = (pm->M11*pv->x + pm->M21*pv->y + pm->M31*pv->z + pm->M41)/norm;
        out.y = (pm->M12*pv->x + pm->M22*pv->y + pm->M32*pv->z + pm->M42)/norm;
        out.z = (pm->M13*pv->x + pm->M23*pv->y + pm->M33*pv->z + pm->M43)/norm;
    }
    else
    {
        out.x = 0.0;
        out.y = 0.0;
        out.z = 0.0;
    }

    *pOut = out;
    return pOut;
}

bool EditAxis::RayPick(float x, float y, bool& select_x, bool& select_y, bool& select_z)
{
    select_x = false;
    select_y = false;
    select_z = false;

    IScene* pScene = IWorld::Instance()->GetScene();
    RenderContext* pRenderCxt = pScene->GetRenderCxt();

    const YY::Mat4f& mtxProj = pRenderCxt->GetProjMatrix();
    YY::Mat4f proj_inverse = mtxProj;
    proj_inverse.Inverse();

    const YY::Mat4f& view = pRenderCxt->GetViewMatrix();
    YY::Mat4f view_inverse = view;
    view_inverse.Inverse();


    float width = pRenderCxt->GetDeviceWidth();
    float height = pRenderCxt->GetDeviceHeight();

    // viewport coordinate: mouse x,y -----> ndc coordinate : Normalised Device Coordinates
    float x0 = (2.0f * x) / width - 1.0f;  
    float y0 = 1.0f - (2.0f * y) / height;  
    float z0 = 1.0f; 
    YY::Vec3f ray_nds = YY::Vec3f (x0, y0, z0);

    // clip coordinate:裁剪坐标
    YY::Vec4f ray_clip;
    ray_clip.x = ray_nds.x;
    ray_clip.y = ray_nds.y;
    ray_clip.z = ray_nds.z;
    ray_clip.w = 1.0f;

    // 裁剪坐标----(投影矩阵逆矩阵)----->视觉坐标
    YY::Vec4f ray_eye = proj_inverse * ray_clip;

    // 视觉坐标----(视图矩阵逆矩阵)----->世界坐标
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


//     static IGameObj* pTestLine=NULL;
//     Lines* pComp_Lines=NULL;
//     if(NULL == pTestLine)
//     {
//         IScene* pScene = IWorld::Instance()->GetScene();
//         pTestLine = pScene->CreateObj();
//         pScene->AddObj(pTestLine);
//         pComp_Lines = (Lines*)pTestLine->AddComp("Lines");
//     }
// 
//     pComp_Lines = (Lines*)pTestLine->GetComp("Lines");
//     YY::Vec3f ray_dst(ray_world.x, ray_world.y, ray_world.z);
//     pComp_Lines->DrawSingleLine(ray_orig, ray_dst, 0xFF00FF00);


    YY::Vec3f pos = GetOwner()->GetTransform()->GetPos();
    YY::Mat4f mat;
    mat.Translate(pos.x, pos.y, pos.z);

    // 附加上绑定物体的旋转阵
    //BindVisualRotate(mat);

    float distance = 1e8;

    size_t index = 0;

    YY::Vec3f v1;
    YY::Vec3f v2;
    YY::Vec3f v3;

    // 逐个三角形做碰撞检测
    for (size_t k = 0; k < m_nPickTriangleNum; k++)
    {
        FmVec3TransformCoord(&v1, &m_PickVertex[k * 3 + 0].pos, &mat);
        FmVec3TransformCoord(&v2, &m_PickVertex[k * 3 + 1].pos, &mat);
        FmVec3TransformCoord(&v3, &m_PickVertex[k * 3 + 2].pos, &mat);

        float t;
        float u;
        float v;

        // 射线与三角形相交
        if (FmIntersectTriEx(&v1, &v2, &v3, &ray_orig, &ray_dir, 
            &u, &v, &t))
        {
            if ((t >= 0.0f) && (t <= 1.0f) && (t < distance))
            {
                // 距离最近的碰撞点
                distance = t;

                index = k;
            }
        }
    }

    if (distance <= 1.0f)
    {
        unsigned int flags = m_nPickFlags[index];

        select_x = (flags & SELECT_X) > 0;
        select_y = (flags & SELECT_Y) > 0;
        select_z = (flags & SELECT_Z) > 0;
    }

    return (distance <= 1.0f);
}



void EditAxis::SetSelectX(bool value)
{ 
    m_bSelectX = value; 

    InitVertex();
}

bool EditAxis::GetSelectX() const
{ 
    return m_bSelectX; 
}

void EditAxis::SetSelectY(bool value) 
{ 
    m_bSelectY = value; 

    InitVertex();
}

bool EditAxis::GetSelectY() const 
{ 
    return m_bSelectY; 
}

void EditAxis::SetSelectZ(bool value) 
{ 
    m_bSelectZ = value; 

    InitVertex();
}

bool EditAxis::GetSelectZ() const 
{ 
    return m_bSelectZ; 
}