/************************************************************************/
/* 
@author:    junliang
@brief:     Mesh
@time:      6/23/2016
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/model/i_mesh.h"

NS_YY_BEGIN

void InitVertex(Vertex* pVertex);
void InitVertex(Vertex* pVertex, float x_, float y_, float z_,float u_, float v_);

void SubMeshDraw(SubMesh* pSM, int nTexID, IShader* tri_Shader);
void SubMeshSetup(SubMesh* pSM);


// ��ǰģ�͵�����
class Mesh : public IMesh
{
public:
    Mesh();
    ~Mesh(){}
    void Load(const std::string& path, const std::string& file);
    void draw(IShader* tri_Shader);

    void AddSubMesh(const SubMesh& sm)
    {
        m_subMeshes.push_back(sm);
        subMesh_setup();
    }

    SubMesh* GetSubMesh(int nIndex)
    {
        throw_assert(nIndex>=0&&nIndex <= m_subMeshes.size(), "parameter check.");
        return &m_subMeshes[nIndex];
    }

    void CalNormalLines(SubMesh* pSM);


    virtual AABB GetAABB();
    void SetAABB(const AABB &aabb);
    void CalculateAABB();

private:
    void LoadFromFile(const std::string& file);
    void subMesh_setup();
private:
    std::string m_path;
    std::vector<SubMesh> m_subMeshes;
    AABB m_aabb;


    //YY_PROP_GET_SET(bool, DiffuseMap);        // ��������ͼ
    //YY_PROP_GET_SET(bool, NormalMap);        // ������ͼ
    //YY_PROP_GET_SET(bool, GlossMap);        // ������ͼ
    //YY_PROP_GET_SET(bool, WireFrame);        // ����ʾ��ͼ��ֻ��ʾmesh������
    //YY_PROP_GET_SET(bool, DrawNormal);        // �Ƿ���ʾ����
    //YY_PROP_GET_SET(bool, DrawSkeleton);    // �Ƿ���ʾ����
    //YY_PROP_GET_SET(bool, DrawBounding);    // �Ƿ���ʾ��Χ��
};


NS_YY_END