/************************************************************************/
/* 
@author:    junliang
@brief:     mesh
@time:      6/23/2016
*/
/************************************************************************/
#pragma once

#include <string>
#include <vector>

// ��̬mesh�Ķ�������
struct MD_Vertex_S
{
    float x,y,z;
    float u,v;
    float nx,ny,nz;

    MD_Vertex_S(float x_, float y_, float z_,float u_, float v_):
    x(x_),y(y_),z(z_),u(u_),v(v_){}
};

#define MAX_BONE_EFFECT_NUM 4
struct MD_Vertex
{
    int vertIndex;            // ��3dmax�еĶ�������
    int texCoordIndex;        // ��3dmax�е���������

    float x,y,z;
    float u,v;
    float nx,ny,nz;

    // skinning
    float bone_weight[MAX_BONE_EFFECT_NUM];
    int bone_id[MAX_BONE_EFFECT_NUM];

    MD_Vertex()
    {
        memset(this, 0, sizeof(MD_Vertex));
    }
};

struct MD_Face
{
    unsigned int indices[3];
};


struct MD_Material
{
    std::string diffuse;
    std::string opacity;
    bool TwoSided;        // �Ƿ�˫��
    float Transparency;    // ��͸����
};

struct MD_SubMesh
{
    std::string name;
    std::string strMatName;
    int hasAnim;    // true means has animation.
    MD_Material material;
    std::vector<MD_Face> faces;
    std::vector<MD_Vertex> vertices;

    MD_SubMesh():hasAnim(0){}
};


struct MD_Mesh
{
    std::vector<MD_SubMesh> subMeshes;
};