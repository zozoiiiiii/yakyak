#include "comp_skybox.h"
#include "texture/cg_texture_mgr.h"
#include "texture/cg_texture.h"
#include "cg.h"


void Comp_SkyBox::OnCreate(const YY::VarList& args)
{
    m_pSubMesh = NULL;
    m_pComp_Camera = NULL;
}

void Comp_SkyBox::OnDestroy()
{
}

void Comp_SkyBox::LoadTex(const std::string& PosXFilename,
                          const std::string& NegXFilename,
                          const std::string& PosYFilename,
                          const std::string& NegYFilename,
                          const std::string& PosZFilename,
                          const std::string& NegZFilename)
{
    m_nTexID_Cube = YY::LoadCubeMapTexture(PosXFilename,NegXFilename,PosYFilename,NegYFilename,PosZFilename,NegZFilename);

    CreateMesh();
    m_pComp_Camera = (Comp_Camera*)GetOwner()->GetComp(YY_COMP_CAMERA_CREATE_NAME);
}


void Comp_SkyBox::CreateMesh()
{
    YY::Vertex vertex[8];
    vertex[0] = YY::Vertex(-1,-1,1,  0,0);
    vertex[1] = YY::Vertex(1,-1,1,   1,0);
    vertex[2] = YY::Vertex(-1,1,1,   0,1);
    vertex[3] = YY::Vertex(1,1,1,    1,1);
    vertex[4] = YY::Vertex(-1,-1,-1, 0,0);
    vertex[5] = YY::Vertex(1,-1,-1,  1,0);
    vertex[6] = YY::Vertex(-1,1,-1,  0,1);
    vertex[7] = YY::Vertex(1,1,-1,   1,1);


    unsigned int indices[]={2,0,1,2,1,3,3,1,5,3,5,7,7,5,4,7,4,6,6,4,0,6,0,2,6,2,3,6,3,7,5,1,0,5,0,4
    };



    YY::SubMesh* pSM = new YY::SubMesh;
    m_pSubMesh = pSM;
    int nVertexNum = sizeof(vertex)/sizeof(YY::Vertex);
    for(int i=0; i<nVertexNum; i++)
    {
        YY::Vertex tmpVert = vertex[i];
        pSM->vertices.push_back(tmpVert);
    }

    int nIndexNum = sizeof(indices)/sizeof(unsigned int);
    for(int i=0; i<nIndexNum; i++)
    {
        pSM->indices.push_back(indices[i]);
    }

    pSM->setup_Mesh();
}


void Comp_SkyBox::OnRender(RenderContext* pCxt)
{
    throw_assert(NULL!= m_pSubMesh, "null check.");
    throw_assert(NULL!= m_pComp_Camera, "null check.");

    //std::string strVsh = get_res_path() + "shader/sky_box.vsh";
    //std::string strFsh = get_res_path() + "shader/sky_box.fsh";
    //static Shader tri_Shader(strVsh.c_str(), strFsh.c_str());

    Shader* pShader = pCxt->pShader;
    pShader->Begin();


    // 视变换矩阵 移除translate部分
    YY::Mat4f view = pCxt->viewM;
    //pCxt->GetViewMatrixWithOutTransform(&view);

    // 投影矩阵
    YY::Mat4f projection = m_pComp_Camera->GetPersp();

    pShader->SetUniformMat4fv("view", 1, &view.m[0][0] );
    pShader->SetUniformMat4fv("projection", 1, &projection.m[0][0] );


    GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    // Bind the texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_nTexID_Cube);
    pShader->SetUniform1i("tex_sky",1);

    // 激活vbo对象
    glBindBuffer(GL_ARRAY_BUFFER, m_pSubMesh->VBO);

    // position
    int offset = 0;
    int vertexLocation = glGetAttribLocation (pShader->GetProgramHandle(),"i_pos");
    if(vertexLocation>=0)
    {
        glEnableVertexAttribArray (vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(YY::Vertex), (const void *)offset);
    }


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pSubMesh->EBO);
    glDrawElements(GL_TRIANGLES, m_pSubMesh->indices.size(), GL_UNSIGNED_INT, 0);


    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);

    //m_pSubMesh->draw(&tri_Shader);

    pShader->End();
}