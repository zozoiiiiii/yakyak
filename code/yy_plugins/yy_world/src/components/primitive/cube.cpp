#include "cube.h"
#include "yy_world/inc/i_world.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

// 0xFFFF0000 red
// 0xFF00FF00 green
// 0xFF0000FF blue

void Cube::OnCreate()
{
    m_color = 0xFFFF0000;

    m_pMesh = NULL;
}


void Cube::Init(int width, int height, int depth)
{
	m_width=width;
	m_height=height;
	m_depth=depth;
}

void Cube::OnDestroy()
{
    
}


void Cube::SetColor(sint32 color)
{
    m_color = color;
    SetupMesh();
}

void Cube::SetupMesh()
{
    throw_assert(NULL == m_pMesh, "only setup one time.");

    IModelRes* pModelRes = IRender::Instance()->GetModelResMgr();
    m_pMesh = pModelRes->CreateMesh();

    float halfWidth = m_width/2.0f;
    float halfDepth = m_height/2.0f;
    float halfHeight = m_depth/2.0f;

    YY::Vertex vertices[24] = {
        // Vertex data for face 0
        YY::Vertex(-1.0f *halfWidth, -1.0f * halfHeight,  1.0f * halfDepth, 0.0f, 0.0f, m_color),  // v0
        YY::Vertex( 1.0f *halfWidth, -1.0f * halfHeight,  1.0f * halfDepth, 0.33f, 0.0f, m_color), // v1
        YY::Vertex(-1.0f *halfWidth,  1.0f * halfHeight,  1.0f * halfDepth, 0.0f, 0.5f, m_color),  // v2
        YY::Vertex( 1.0f *halfWidth,  1.0f * halfHeight,  1.0f * halfDepth, 0.33f, 0.5f, m_color), // v3

        // Vertex data for face 1
        YY::Vertex( 1.0f *halfWidth, -1.0f * halfHeight,  1.0f * halfDepth,  0.0f, 0.5f, m_color), // v4
        YY::Vertex( 1.0f *halfWidth, -1.0f * halfHeight, -1.0f * halfDepth, 0.33f, 0.5f, m_color), // v5
        YY::Vertex( 1.0f *halfWidth,  1.0f * halfHeight,  1.0f * halfDepth, 0.0f, 1.0f, m_color),  // v6
        YY::Vertex( 1.0f *halfWidth,  1.0f * halfHeight, -1.0f * halfDepth, 0.33f, 1.0f, m_color), // v7

        // Vertex data for face 2
        YY::Vertex( 1.0f *halfWidth, -1.0f * halfHeight, -1.0f * halfDepth, 0.66f, 0.5f, m_color), // v8
        YY::Vertex(-1.0f *halfWidth, -1.0f * halfHeight, -1.0f * halfDepth, 1.0f, 0.5f, m_color),  // v9
        YY::Vertex( 1.0f *halfWidth,  1.0f * halfHeight, -1.0f * halfDepth, 0.66f, 1.0f, m_color), // v10
        YY::Vertex(-1.0f *halfWidth,  1.0f * halfHeight, -1.0f * halfDepth, 1.0f, 1.0f, m_color),  // v11

        // Vertex data for face 3
        YY::Vertex(-1.0f *halfWidth, -1.0f * halfHeight, -1.0f * halfDepth, 0.66f, 0.0f, m_color), // v12
        YY::Vertex(-1.0f *halfWidth, -1.0f * halfHeight,  1.0f * halfDepth, 1.0f, 0.0f, m_color),  // v13
        YY::Vertex(-1.0f *halfWidth,  1.0f * halfHeight, -1.0f * halfDepth, 0.66f, 0.5f, m_color), // v14
        YY::Vertex(-1.0f *halfWidth,  1.0f * halfHeight,  1.0f * halfDepth, 1.0f, 0.5f, m_color),  // v15

        // Vertex data for face 4
        YY::Vertex(-1.0f *halfWidth, -1.0f * halfHeight, -1.0f * halfDepth, 0.33f, 0.0f, m_color), // v16
        YY::Vertex( 1.0f *halfWidth, -1.0f * halfHeight, -1.0f * halfDepth, 0.66f, 0.0f, m_color), // v17
        YY::Vertex(-1.0f *halfWidth, -1.0f * halfHeight,  1.0f * halfDepth, 0.33f, 0.5f, m_color), // v18
        YY::Vertex( 1.0f *halfWidth, -1.0f * halfHeight,  1.0f * halfDepth, 0.66f, 0.5f, m_color), // v19

        // Vertex data for face 5
        YY::Vertex(-1.0f *halfWidth,  1.0f * halfHeight,  1.0f * halfDepth, 0.33f, 0.5f, m_color), // v20
        YY::Vertex( 1.0f *halfWidth,  1.0f * halfHeight,  1.0f * halfDepth, 0.66f, 0.5f, m_color), // v21
        YY::Vertex(-1.0f *halfWidth,  1.0f * halfHeight, -1.0f * halfDepth, 0.33f, 1.0f, m_color), // v22
        YY::Vertex( 1.0f *halfWidth,  1.0f * halfHeight, -1.0f * halfDepth, 0.66f, 1.0f, m_color)  // v23
    };


    YY::SubMesh sm;
    for(int i=0; i<24; i++)
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
        0,  1,  2,  1,  3,  2,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        4,  5,  6,  5,  7,  6, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
        8,  9,  10, 9, 11, 10, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 13, 14, 13, 15, 14, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 17, 18, 17, 19, 18, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 21, 22, 21, 23, 22,      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    for(int i=0; i<36; i++)
    {
        unsigned short index = indices[i];
        sm.indices.push_back(index);
    }

    m_pMesh->CalNormalLines(&sm);
    m_pMesh->AddSubMesh(sm);


    // aabb
	IGameObj* pGameObj = FindGameObj();
	if(pGameObj)
		pGameObj->SetAABB(m_pMesh->GetAABB());
}

void Cube::OnRender(IRender* pRender, RenderContext* pCxt)
{
    static IShader* pMeshShader = NULL;
    if(!pMeshShader)
    {
        std::string vsh = pRender->GetResPath() + "shader\\mesh.vsh";
		std::string fsh = pRender->GetResPath() + "shader\\mesh.fsh";
		pMeshShader = IRender::Instance()->GetResMgr()->LoadShader(vsh, fsh);
    }

    // 如果是渲染阴影，则用对应的shader
    IShader* pShader = pMeshShader;
    if(pCxt->nRenderType == RT_Shadow)
        pShader = pCxt->pShader_ShadowMap;

    //告诉gpu一次要执行的命令
    pShader->Begin();


    if(pCxt->nRenderType == RT_Normal)
    {
        YY::Vec3f color = pCxt->ambient_color;
        pShader->SetUniform3f("ambient.color", color.x, color.y, color.z);
        pShader->SetUniform1f("ambient.intensity", pCxt->ambient_intensity);

        // direction light
        color = pCxt->directionLight_color;
        pShader->SetUniform3f("directionLight.color", color.x, color.y, color.z);
        pShader->SetUniform1f("directionLight.intensity", pCxt->directionLight_intensity);
        YY::Vec3f direction = pCxt->directionLight_direction;
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

	IGameObj* pGameObj = FindGameObj();
	if (!pGameObj)
		return;
    YY::Mat4f model;
	pGameObj->GetTransform()->GetTMMatrix(&model);

    //YY::Mat4f mvp = pCxt->projM * pCxt->viewM * model;



    // matrix
    //pShader->SetUniformMat4fv("u_mvp", 1, &mvp.m_data );
    pShader->SetUniformMat4fv("u_view", 1, pCxt->viewMatrix.m_data );
    pShader->SetUniformMat4fv("u_proj", 1, pCxt->projMatrix.m_data );
    pShader->SetUniformMat4fv("u_model", 1, model.m_data );

    m_pMesh->draw(pShader);

    // 告诉gpu结束该命令
    pShader->End();
}
