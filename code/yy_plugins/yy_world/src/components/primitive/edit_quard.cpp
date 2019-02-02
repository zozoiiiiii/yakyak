#include "edit_quard.h"
#include "lines.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

void EditQuard::OnCreate()
{
    m_pMesh=NULL;

    SetUpMesh();
}

void EditQuard::SetUpMesh()
{
    throw_assert(NULL == m_pMesh, "only setup one time.");

    IScene* pScene = IWorld::Instance()->GetScene();
    IModelRes* pModelRes = pScene->GetRender()->GetModelResMgr();
    m_pMesh = pModelRes->CreateMesh();


    YY::Vertex vertices[4] = {
        YY::Vertex(-1.0,-1.0,0.0,    0.0,0.0),
        YY::Vertex(1.0,-1.0,0.0,     1.0,0.0),
        YY::Vertex(1.0,1.0,0.0,      1.0,1.0),
        YY::Vertex(-1.0,1.0,0.0,     0.0,1.0)
    };


    YY::SubMesh sm;
    sm.material.diffuse = "texture/grey.bmp";
    for(int i=0; i<4; i++)
    {
        const YY::Vertex& v = vertices[i];
        sm.vertices.push_back(v);
    }

    unsigned short indices[] = {0,1,2,0,2,3};

    for(int i=0; i<6; i++)
    {
        unsigned short index = indices[i];
        sm.indices.push_back(index);
    }

    m_pMesh->AddSubMesh(sm);
}


void EditQuard::OnRender(RenderContext* pCxt)
{
    glDisable(GL_DEPTH_TEST);

    static IShader* pMeshShader = NULL;
    if(!pMeshShader)
    {
        IScene* pScene = IWorld::Instance()->GetScene();
        pMeshShader = pScene->GetRender()->CreateShader();
        //std::string vsh = IWorld::Instance()->GetResPath() + "shader\\light_pass.vsh";
        //std::string fsh = IWorld::Instance()->GetResPath() + "shader\\light_pass_dir.fsh";

        std::string vsh = IWorld::Instance()->GetResPath() + "shader\\posteffect.vsh";
        std::string fsh = IWorld::Instance()->GetResPath() + "shader\\posteffect.fsh";
        pMeshShader->Load(vsh.c_str(), fsh.c_str());
    }

    IShader* pShader = pMeshShader;

    //告诉gpu一次要执行的命令
    pShader->Begin();


    // 使用深度图的纹理，对应前面BindForReading的纹理索引（考虑放到scene类里面）
    //pShader->SetUniform1i("ShadowMap", 2);
    pShader->SetUniform1i("ColorTexture", 3);


    throw_assert(NULL!=m_pMesh, "null check.");
    m_pMesh->draw(pShader);

    // 告诉gpu结束该命令
    pShader->End();
    glEnable(GL_DEPTH_TEST);
}


