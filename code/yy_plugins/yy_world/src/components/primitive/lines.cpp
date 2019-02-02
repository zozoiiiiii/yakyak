#include "lines.h"
#include "yy_world/inc/i_world.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

// 0xFFFF0000 red
// 0xFF00FF00 green
// 0xFF0000FF blue

void Lines::OnCreate()
{
    m_nLineNum = 0;
    glGenBuffers(1, &m_VBO);
}


void Lines::DrawLine(const YY::Vec3f& from, const YY::Vec3f& to, unsigned int color)
{
    m_vexData[m_nLineNum*2].pos = from;
    m_vexData[m_nLineNum*2].color = color;
    m_vexData[m_nLineNum*2+1].pos = to;
    m_vexData[m_nLineNum*2+1].color = color;

    m_nLineNum++;
}


void Lines::DrawSingleLine(const YY::Vec3f& from, const YY::Vec3f& to, unsigned int color)
{
    m_nLineNum = 1;
    m_vexData[0].pos = from;
    m_vexData[0].color = color;
    m_vexData[1].pos = to;
    m_vexData[1].color = color;

}

void Lines::OnDestroy()
{
}

void Lines::OnRender(RenderContext* pCxt)
{
    if(pCxt->nRenderType != RT_Normal)
        return;




    // update dynamic vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_nLineNum*2*sizeof(vertex_line_t), &m_vexData[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    static IShader* pShader = NULL;
    if(NULL == pShader)
    {
        std::string vsh = GetRender()->GetResPath() + "shader\\edit_line.vsh";
        std::string fsh = GetRender()->GetResPath() + "shader\\edit_line.fsh";
        pShader = GetRender()->GetResMgr()->LoadShader(vsh, fsh);
    }


    //告诉gpu一次要执行的命令
    pShader->Begin();

    YY::Mat4f model;
    GetOwner()->GetTransform()->GetTMMatrix(&model);

    YY::Mat4f mvp = pCxt->projMatrix * pCxt->viewMatrix * model;
    pShader->SetUniformMat4fv("c_mtxWVP", 1, mvp.m_data);

    // 激活vbo对象
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
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

    glDrawArrays(GL_LINES, 0, m_nLineNum*2);
    glLineWidth(1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // 告诉gpu结束该命令
    pShader->End();
}

