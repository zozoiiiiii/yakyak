#include "gridlines.h"

// 0xFFFF0000 red
// 0xFF00FF00 green
// 0xFF0000FF blue

void GridLines::OnCreate()
{
    int i = 0;
    for (i = 0; i < MAX_VERTEXT_COUNT / 4; i++)
    {
        float tmp_x = -20.0f + i * 2.0f;
        vexData[i * 2].x = tmp_x;
        vexData[i * 2].y = 0.0f;
        vexData[i * 2].z = -20.0f;
        vexData[i * 2].color = 0xffffffff;
        vexData[i * 2 + 1].x = tmp_x;
        vexData[i * 2 + 1].y = 0.0f;
        vexData[i * 2 + 1].z = 20.0f;
        vexData[i * 2 + 1].color = 0xffffffff;

        // z axis : blue
        if( YY::FloatEqualZero(tmp_x))
        {
            vexData[i*2].color = 0xffffffff;
            vexData[i*2+1].color = 0xFF0000FF;
        }
    }

    i = 0;
    for (i = 0; i < MAX_VERTEXT_COUNT / 4; i++)
    {
        float tmp_z = -20.0f + i * 2.0f;
        vexData[i * 2 + MAX_VERTEXT_COUNT / 2].x = -20.0f;
        vexData[i * 2 + MAX_VERTEXT_COUNT / 2].y = 0.0f;
        vexData[i * 2 + MAX_VERTEXT_COUNT / 2].z = tmp_z;
        vexData[i * 2 + MAX_VERTEXT_COUNT / 2].color = 0xffffffff;
        vexData[i * 2 + 1 + MAX_VERTEXT_COUNT / 2].x = 20.0f;
        vexData[i * 2 + 1 + MAX_VERTEXT_COUNT / 2].y = 0.0f;
        vexData[i * 2 + 1 + MAX_VERTEXT_COUNT / 2].z = tmp_z;
        vexData[i * 2 + 1 + MAX_VERTEXT_COUNT / 2].color = 0xffffffff;

        // x axis : red
        if( YY::FloatEqualZero(tmp_z))
        {
            vexData[i*2 + MAX_VERTEXT_COUNT / 2].color = 0xffffffff;
            vexData[i*2+1 + MAX_VERTEXT_COUNT / 2].color = 0xFFFF0000;
        }
    }


    glGenBuffers(1, &this->VBO);

    // 结构体内部变量连续，所以可以直接把一个作为缓冲数据的一大列Vertex结构体的指针传递过去

    // 绑定顶点数组缓冲
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    // 分配内存空间，并且存入数据
    //glBufferData(GL_ARRAY_BUFFER, MAX_VERTEXT_COUNT * sizeof(vertex_line_t), &vexData[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTEXT_COUNT * sizeof(vertex_line_t), &vexData[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GridLines::OnDestroy()
{
    
}

void GridLines::OnRender(RenderContext* pCxt)
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

    YY::Mat4f mvp = pCxt->GetProjMatrix() * pCxt->GetViewMatrix() * model;
    pShader->SetUniformMat4fv("c_mtxWVP", 1, mvp.m_data);

    // 激活vbo对象
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
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

    glDrawArrays(GL_LINES, 0, MAX_VERTEXT_COUNT);
    glLineWidth(1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // 告诉gpu结束该命令
    pShader->End();
}

