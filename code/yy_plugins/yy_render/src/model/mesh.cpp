#include "Mesh.h"
#include "tinyxml/tinyxml.h"
#include "yy_render/inc/res/i_res_mgr.h"
#include "yy_render/inc/res/i_texture.h"
#include "../platform.h"
#include "model_res.h"
#include "core/inc/yy_file.h"
NS_YY_BEGIN

void InitVertex(Vertex* pVertex, float x_, float y_, float z_,float u_, float v_)
{
    pVertex->position.x =x_;
    pVertex->position.y=y_;
    pVertex->position.z=z_;
    pVertex->tex_coords.x=u_;
    pVertex->tex_coords.y=v_;

    pVertex->count = 0;
    for(int i=0; i<4; i++)
    {
        pVertex->bone_weight[i]=0.0f;
        pVertex->bone_id[i]=0.0f;
    }
}

void InitVertex(Vertex* pVertex)
{
    pVertex->count = 0;
    for(int i=0; i<4; i++)
    {
        pVertex->bone_weight[i]=0.0f;
        pVertex->bone_id[i]=0.0f;
    }
}


void SubMeshDraw(SubMesh* pSM, int nTexID, IShader* tri_Shader)
{
    // ѡ���Կ��е�ǰ��Ծ������Ԫ
    glActiveTexture(GL_TEXTURE0);

    // ������Ŀ�꣬������ǰ��ѡ��Ļ�Ծ������Ԫ
    if(nTexID>0)
    {
        glBindTexture(GL_TEXTURE_2D, nTexID);
        tri_Shader->SetUniform1i("texture", 0);
    }

    // ��subMesh�Ƿ��ж���
    int u_hasAnimation=glGetUniformLocation(tri_Shader->GetProgramHandle(), "u_hasAnimation");
    if(u_hasAnimation>=0)
    {
        // ȡ����������
         //hasAnim = false;
        if(pSM->hasAnim)
            glUniform1i(u_hasAnimation, 1);
        else
            glUniform1i(u_hasAnimation, 0);
    }


    //�ƶ�����
    //m_anim->setAnimateTime(m_anim->animateTime()+0.01);
    //m_anim->render(tri_Shader);

    // ����vbo����
    glBindBuffer(GL_ARRAY_BUFFER, pSM->VBO);

    // position
    int offset = 0;
    int vertexLocation = glGetAttribLocation (tri_Shader->GetProgramHandle(),"i_pos");
    if(vertexLocation>=0)
    {
        glEnableVertexAttribArray (vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offset);
    }

    // normal line
    offset = offsetof(Vertex, normal);
    int normalLineLocation = glGetAttribLocation (tri_Shader->GetProgramHandle(),"a_normal_line");
    if(normalLineLocation>=0)
    {
        glEnableVertexAttribArray(normalLineLocation);
        glVertexAttribPointer(normalLineLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offset);
    }

    // texture coordinate
    offset = offsetof(Vertex, tex_coords);
    int texcoordLocation = glGetAttribLocation (tri_Shader->GetProgramHandle(),"i_texcoord");
    if(texcoordLocation>=0)
    {
        glEnableVertexAttribArray(texcoordLocation);
        glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offset);
    }

    //bone id
    offset = offsetof(Vertex, bone_id);
    int BoneId = glGetAttribLocation (tri_Shader->GetProgramHandle(),"BONE_ID");
    if(BoneId>=0)
    {
        glEnableVertexAttribArray(BoneId);
        glVertexAttribPointer(BoneId,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void *)offset);
    }

    //bone weight
    offset = offsetof(Vertex, bone_weight);
    int BoneWeight = glGetAttribLocation (tri_Shader->GetProgramHandle(),"BONE_WEIGHT");
    if(BoneWeight>=0)
    {
        glEnableVertexAttribArray(BoneWeight);
        glVertexAttribPointer(BoneWeight,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const void *)offset);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSM->EBO);

    GLenum drawType = GL_TRIANGLES;
    glDrawElements(drawType, pSM->indices.size(), GL_UNSIGNED_INT, 0);
}

void SubMeshSetup(SubMesh* pSM)
{
    glGenBuffers(1, &pSM->VBO);
    glGenBuffers(1, &pSM->EBO);

    // �ṹ���ڲ��������������Կ���ֱ�Ӱ�һ����Ϊ�������ݵ�һ����Vertex�ṹ���ָ�봫�ݹ�ȥ

    // �󶨶������黺��
    glBindBuffer(GL_ARRAY_BUFFER, pSM->VBO);

    // �����ڴ�ռ䣬���Ҵ�������
    glBufferData(GL_ARRAY_BUFFER, pSM->vertices.size() * sizeof(Vertex), 
        &pSM->vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSM->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pSM->indices.size() * sizeof(unsigned int), 
        &pSM->indices[0], GL_STATIC_DRAW);
}


Mesh::Mesh()
{
    //SetDiffuseMap(false);
    //SetNormalMap(false);
    //SetGlossMap(false);
    //SetWireFrame(false);
    //SetDrawNormal(false);
    //SetDrawSkeleton(false);
    //SetDrawBounding(false);
}

void Mesh::Load(const std::string& path, const std::string& file)
{
    std::string res_path = ModelRes::Instance()->GetResPath();
    m_path = path;
    LoadFromFile(res_path + m_path + file);
    subMesh_setup();
}


void Mesh::subMesh_setup()
{
    for(int i=0; i<m_subMeshes.size(); i++)
    {
        SubMesh* sm = &m_subMeshes[i];
        SubMeshSetup(sm);
    }
}

void Mesh::LoadFromFile(const std::string& file)
{
    FILE *infile = fopen(file.c_str(), "rt");
    throw_assert(infile, "open file faile:"<<file);

    // sub Mesh
    int SubMesh_num=0;
    fscanf(infile, "submesh num: %d\n", &SubMesh_num);
    m_subMeshes.resize(SubMesh_num);
    for (int i = 0; i < SubMesh_num; i++)
    {
        SubMesh& sm = m_subMeshes[i];

        int tmpSMIndex;
        fscanf(infile, "this is sub mesh:%d, has anim: %d\n", &tmpSMIndex, &sm.hasAnim);

        int index;
        int vert_num;

        // [texture name]
        int diffuseLen;
        char diffuseName[200]={0};
        int opacityLen;
        char opacityName[200]={0};

        fscanf(infile, "( diffuse:%d %s )( opacity:%d %s )\n", &diffuseLen, diffuseName, &opacityLen, opacityName);
        if(diffuseLen > 0 && diffuseLen < 200)
            sm.material.diffuse = m_path + diffuseName;

        if(opacityLen > 0 && opacityLen < 200)
            sm.material.opacity = opacityName;

        fscanf(infile, "vertex num: %d\n", &vert_num);
        sm.vertices.resize(vert_num);
        for (int j = 0; j < vert_num; j++)
        {
            int vertex_index = -1;
            int weight_num = 0;
            Vertex& vert = sm.vertices[j];

            int bone_id[4];
            fscanf(infile, "%d (%f %f %f) (%f %f %f) (%f %f) (%f %f %f %f) (%d %d %d %d)\n",
                &vertex_index,
                &vert.position.x,
                &vert.position.y,
                &vert.position.z,
                &vert.normal.x,
                &vert.normal.y,
                &vert.normal.z,
                &vert.tex_coords.x,
                &vert.tex_coords.y,
                &vert.bone_weight[0],
                &vert.bone_weight[1],
                &vert.bone_weight[2],
                &vert.bone_weight[3],
                &bone_id[0],
                &bone_id[1],
                &bone_id[2],
                &bone_id[3]);


            //vert.tex_coords.y = 1.0f - vert.tex_coords.y;
            vert.bone_id[0]=bone_id[0];
            vert.bone_id[1]=bone_id[1];
            vert.bone_id[2]=bone_id[2];
            vert.bone_id[3]=bone_id[3];
            throw_assert(vertex_index == j, "vertex index:"<<vertex_index<<" vs "<<j);
        }
        int numFaces = 0;
        fscanf(infile, "face num: %d\n", &numFaces);
        for (int j = 0; j < numFaces; j++)
        {
            int face_index=-1;
            int numIndices = 0;
            fscanf(infile, "%d %d ", &face_index, &numIndices);
            throw_assert(face_index == j, "face index:"<<face_index<<" vs "<<j);

            throw_assert(numIndices == 3, "every face should has 3 indices:"<<numIndices);

            for (int kk = 0; kk < numIndices; kk++)
            {
                int vert_index;
                fscanf(infile, "%d ", &vert_index);
                sm.indices.push_back(vert_index);
            }
            fscanf(infile, "\n");
        }
    }

    fclose(infile);
}

void Mesh::draw(IShader* tri_Shader)
{
    for(int i=0; i<m_subMeshes.size(); i++)
    {
        SubMesh* sm = &m_subMeshes[i];


        ///////////////////// ����
        // ������ʱֻ����diffuse. û�в��ʵ�ʱ����Ĭ����ͼ sm->material.diffuse = "texture/default.png";
        int texID = -1;
        if(!sm->material.diffuse.empty())
        {
            IResMgr* pResMgr = tri_Shader->GetResMgr();
            ITexture* pRes = pResMgr->LoadTexture(sm->material.diffuse);
            texID = pRes->GetID();
            //texID = texture_mgr::instance()->create(sm->material.diffuse.c_str());
        }

        SubMeshDraw(sm, texID, tri_Shader);
    }
}

void Mesh::CalNormalLines(SubMesh* pSM)
{
    // generate normal line
    int indices_count = pSM->indices.size();
    for (unsigned int i = 0 ; i < indices_count ; i += 3) {
        unsigned int Index0 = pSM->indices[i];
        unsigned int Index1 = pSM->indices[i + 1];
        unsigned int Index2 = pSM->indices[i + 2];
        YY::Vertex* v0 = &pSM->vertices[Index0];
        YY::Vertex* v1 = &pSM->vertices[Index1];
        YY::Vertex* v2 = &pSM->vertices[Index2];

        YY::Vec3f v10 = v1->position - v0->position;
        YY::Vec3f v20 = v2->position - v0->position;

        // ��������˻�÷���
        YY::Vec3f Normal = v10.cross(v20);
        Normal.norm();
        v0->normal += Normal;
        v1->normal += Normal;
        v2->normal += Normal;
    }

    int vertices_count = pSM->vertices.size();
    for (unsigned int i = 0 ; i < vertices_count ; i++) {

        YY::Vertex* v0 = &pSM->vertices[i];
        v0->normal.norm();
    }

}


AABB Mesh::GetAABB()
{
    CalculateAABB();
    return m_aabb;
}

void Mesh::SetAABB(const AABB &aabb)
{
    m_aabb = aabb;
}

void Mesh::CalculateAABB()
{
    int nSubMeshNum = m_subMeshes.size();
    for(int i=0; i<nSubMeshNum; i++)
    {
        const SubMesh& sm = m_subMeshes[i];
        int nVertexNum = sm.vertices.size();
        for(int j=0;j<nVertexNum;j++)
        {
            const Vertex& vx = sm.vertices[j];
            m_aabb.update(vx.position);
        }
    }
}

NS_YY_END