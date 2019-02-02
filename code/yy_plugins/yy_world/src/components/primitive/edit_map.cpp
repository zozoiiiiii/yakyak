#include "edit_map.h"
#include "lines.h"
#include "yy_world/inc/components/i_comp_camera.h"

void EditMap::OnCreate()
{
    m_pMesh=NULL;

}

bool EditMap::SetParams(int w, int h)
{

    m_w =w;
    m_h =h;



    float x2 = -m_w/2.0f;
    float x1 = m_w/2.0f;

    float z1 = -m_h/2.0f;
    float z2 = m_h/2.0f;

    vexData[0].x = x1;       vexData[0].y = 0;         vexData[0].z = z1;  vexData[0].color = 0xFFFF0000;
    vexData[1].x = x2;       vexData[1].y = 0;         vexData[1].z = z1;  vexData[1].color = 0xFFFF0000;
    vexData[2].x = x1;       vexData[2].y = 0;         vexData[2].z = z2;  vexData[2].color = 0xFFFF0000;
    vexData[3].x = x2;       vexData[3].y = 0;         vexData[3].z = z2;  vexData[3].color = 0xFFFF0000;
    vexData[4].x = x1;       vexData[4].y = 0;         vexData[4].z = z2;  vexData[4].color = 0xFFFF0000;
    vexData[5].x = x2;       vexData[5].y = 0;         vexData[5].z = z1;  vexData[5].color = 0xFFFF0000;



    throw_assert(NULL == m_pMesh, "only setup one time.");

    IScene* pScene = IWorld::Instance()->GetScene();
    IModelRes* pModelRes = pScene->GetRender()->GetModelResMgr();
    m_pMesh = pModelRes->CreateMesh();


    unsigned int color = 0xFF00FF00;
    YY::Vertex vertices[6] = {
        YY::Vertex(x1,0,z1,0,0,color),
        YY::Vertex(x2,0,z1,0,0,color),
        YY::Vertex(x1,0,z2,0,0,color),
        YY::Vertex(x2,0,z2,0,0,color),
        YY::Vertex(x1,0,z2,0,0,color),
        YY::Vertex(x2,0,z1,0,0,color),
    };


    YY::SubMesh sm;
    sm.material.diffuse = "texture/grey.bmp";
    for(int i=0; i<6; i++)
    {
        const YY::Vertex& v = vertices[i];
        sm.vertices.push_back(v);
    }


    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    unsigned short indices[] = {
        0,  1,  2,  3,  4,  5,
    };

    for(int i=0; i<6; i++)
    {
        unsigned short index = indices[i];
        sm.indices.push_back(index);
    }

    m_pMesh->CalNormalLines(&sm);
    m_pMesh->AddSubMesh(sm);
	return true;
}


// 正常渲染
void EditMap::OnRender(RenderContext* pCxt)
{
    static IShader* pMeshShader = NULL;
    if(!pMeshShader)
    {
        IScene* pScene = IWorld::Instance()->GetScene();
        pMeshShader = pScene->GetRender()->CreateShader();
        std::string vsh = IWorld::Instance()->GetResPath() + "shader\\mesh.vsh";
        std::string fsh = IWorld::Instance()->GetResPath() + "shader\\mesh.fsh";
        pMeshShader->Load(vsh.c_str(), fsh.c_str());
    }

    // 如果是渲染阴影，则用对应的shader
    IShader* pShader = pMeshShader;
    if(pCxt->GetRenderType() == RT_Shadow)
        pShader = pCxt->GetShadowShader();

    //告诉gpu一次要执行的命令
    pShader->Begin();


    if(pCxt->GetRenderType() == RT_Normal)
    {
        YY::Vec3f color = pCxt->GetColor(ambient_color);
        pShader->SetUniform3f("ambient.color", color.x, color.y, color.z);
        pShader->SetUniform1f("ambient.intensity", pCxt->GetFloat(ambient_intensity));

        // direction light
        color = pCxt->GetColor(directionLight_color);
        pShader->SetUniform3f("directionLight.color", color.x, color.y, color.z);
        pShader->SetUniform1f("directionLight.intensity", pCxt->GetFloat(directionLight_intensity));
        YY::Vec3f direction = pCxt->GetVector(directionLight_direction);
        pShader->SetUniform3f("directionLight.direction", direction.x, direction.y, direction.z);

        // LightVPMatrix
        YY::Mat4f lightView;
        lightView.LookAtRh(direction.norm(), YY::Vec3f(0,1,0));
        lightView.Inverse();

        YY::Mat4f lightProj;
        lightProj.OrthoRh(-25,25,-25,25,0.1f,20.0f);
        YY::Mat4f lightPV = lightProj * lightView;
        pShader->SetUniformMat4fv("LightVPMatrix", 1, lightPV.m_data);



        // 使用深度图的纹理，对应前面BindForReading的纹理索引（考虑放到scene类里面）
        pShader->SetUniform1i("ShadowMap", 2);
    }

    //----------
    throw_assert(NULL!=m_pMesh, "null check.");

    YY::Mat4f model;

    //YY::Mat4f mvp = pCxt->projM * pCxt->viewM * model;



    // matrix
    //pShader->SetUniformMat4fv("u_mvp", 1, &mvp.m_data );
    pShader->SetUniformMat4fv("u_view", 1, pCxt->GetViewMatrix().m_data );
    pShader->SetUniformMat4fv("u_proj", 1, pCxt->GetProjMatrix().m_data );
    pShader->SetUniformMat4fv("u_model", 1, model.m_data );

    m_pMesh->draw(pShader);

    // 告诉gpu结束该命令
    pShader->End();
}



bool EditMap::GetRayInfo(int x, int y, YY::Vec3f& orig, YY::Vec3f& direction)
{

    return true;
}


YY::Vec3f EditMap::GetPos(int index)
{

    YY::Vec3f tmp_v1(vexData[index].x, vexData[index].y, vexData[index].z);
    return tmp_v1;
}



int FmIntersectTriEx11(const YY::Vec3f* pv1,const YY::Vec3f* pv2,const YY::Vec3f* pv3,const YY::Vec3f* pRayPos,
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


bool EditMap::RayPick(int x, int y, float& x_, float& y_, float& z_)
{
    YY::Vec3f orig;
    YY::Vec3f direction;

    IScene* pScene = IWorld::Instance()->GetScene();
    IGameObj* pGameObj_Camera = pScene->GetMainCamera();
    IComp_Camera* pCamera = (IComp_Camera*)pGameObj_Camera->GetComp("Comp_Camera");

    pCamera->GetRayDirection(x,y,direction.x, direction.y, direction.z);
    orig = pGameObj_Camera->GetTransform()->GetPos();


    float x2 = -m_w/2.0f;
    float x1 = m_w/2.0f;

    float z1 = -m_h/2.0f;
    float z2 = m_h/2.0f;

    float distance = 1e8;

    {
        // 三角形做碰撞检测
        YY::Vec3f v1 = GetPos(0);
        YY::Vec3f v2 = GetPos(1);
        YY::Vec3f v3 = GetPos(2);

        float t;
        float u;
        float v;

        // 射线与三角形相交
        if (FmIntersectTriEx11(&v1, &v2, &v3, &orig, &direction, 
            &u, &v, &t))
        {
            if ((t >= 0.0f) && (t <= 1.0f) && (t < distance))
            {
                // 距离最近的碰撞点
                distance = t;
                x_ = u * (x2 - x1) + x1;
                y_ = v1.y;
                z_ = v * (z2 - z1) + z1;
                return true;
            }
        }
    }

    {
        // 三角形做碰撞检测
        YY::Vec3f v1 = GetPos(3);
        YY::Vec3f v2 = GetPos(4);
        YY::Vec3f v3 = GetPos(5);

        float t;
        float u;
        float v;

        // 射线与三角形相交
        if (FmIntersectTriEx11(&v1, &v2, &v3, &orig, &direction, 
            &u, &v, &t))
        {
            if ((t >= 0.0f) && (t <= 1.0f) && (t < distance))
            {
                // 距离最近的碰撞点
                distance = t;
                x_ = (1-u) * (x2 - x1) + x1;
                y_ = v1.y;
                z_ = (1-v) * (z2 - z1) + z1;
                return true;
            }
        }
    }
    return false;
}
