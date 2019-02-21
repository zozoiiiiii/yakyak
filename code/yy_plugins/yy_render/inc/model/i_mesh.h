/************************************************************************/
/* 
@author:    junliang
@brief:     Mesh
@time:      6/23/2016
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include <vector>
#include "../res/i_Shader.h"
#include "math/inc/yy_aabb.h"
#include "core/inc/yy_file.h"

NS_YY_BEGIN
struct Vertex
{
    YY::Vec3f position;            // model space
    YY::Vec2f tex_coords;
    YY::Vec3f normal;
    //YY::Vec3f tangent;

    // skinning
    int count;
    float bone_weight[4];
	float bone_id[4];
	unsigned int color;            // if no diffuse texture, will use this color.

    Vertex()
    {
        this->count = 0;
        for(int i=0; i<4; i++)
        {
            this->bone_weight[i]=0.0f;
            this->bone_id[i]=0.0f;
        }
    }

    Vertex(float x, float y, float z, float tx, float ty)
        :position(x,y,z), tex_coords(tx,ty){

            this->count = 0;
            for(int i=0; i<4; i++)
            {
                this->bone_weight[i]=0.0f;
                this->bone_id[i]=0.0f;
            }
    }

    Vertex(float x, float y, float z, float tx, float ty, unsigned int color_)
        :position(x,y,z), tex_coords(tx,ty),color(color_){

            this->count = 0;
            for(int i=0; i<4; i++)
            {
                this->bone_weight[i]=0.0f;
                this->bone_id[i]=0.0f;
            }
    }
};

struct Material
{
    std::string diffuse;
    std::string opacity;
};

// 当前模型的一部分, 只使用一种纹理
struct SubMesh
{
    int hasAnim;    // 非0表示有动画
    Material material;
    std::vector<int> indices;
    std::vector<Vertex> vertices;

    //render
    unsigned int VBO, EBO;

    SubMesh():hasAnim(0){}
};

// 当前模型的网格
class IMesh
{
public:
    virtual void Load(const std::string& path, const std::string& file) = 0;
    virtual void draw(IShader* tri_Shader) = 0; 
    virtual void AddSubMesh(const SubMesh& sm) = 0;
    virtual SubMesh* GetSubMesh(int nIndex) = 0;
    virtual void CalNormalLines(SubMesh* pSM) = 0;
    virtual AABB GetAABB() = 0;
};


NS_YY_END