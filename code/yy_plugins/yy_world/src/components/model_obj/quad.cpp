#include "quad.h"
#include "texture/stb_image_aug.h"
#include "tiny_obj_loader.h"

#define MAP_SCALE 0.1        // 地图缩放
#define MAP_HEIGHT 1        // 地图最大高差

void Quad::OnCreate()
{
}

void Quad::OnDestroy()
{
}


void Quad::InitModel(const std::string& strModelFile, const std::string& strModelDir)
{
    throw_assert(NULL==m_mesh, "null check.");
    m_mesh = GetOwner()->GetRender()->GetModelResMgr()->CreateMesh();

    //模型文件信息
    std::vector<tinyobj::shape_t> m_file_shapes;
    std::vector<tinyobj::material_t> m_file_materials;

    //加载模型
    std::string err = tinyobj::LoadObj(m_file_shapes, m_file_materials, strModelFile.c_str(), strModelDir.c_str());
    throw_assert (err.empty(), "load obj failed.");

    int nSubMeshNum = m_file_shapes.size();
    int nMatNum = m_file_materials.size();

    for(int i=0; i<m_file_shapes.size(); i++)
    {
        tinyobj::mesh_t& shape_mesh=m_file_shapes[i].mesh;

        YY::SubMesh sm;
        for(int j=0; j<shape_mesh.indices.size(); j++)
        {
            unsigned int index = shape_mesh.indices[j];
            sm.indices.push_back(index);
        }

        int nVertNum = shape_mesh.positions.size() / 3;
        int nTexNum = shape_mesh.texcoords.size() / 2;
        throw_assert(nVertNum == nTexNum, "check.");

        sm.vertices.resize(nVertNum);
        for(int j=0; j<nVertNum; j++)
        {
            YY::Vertex* pVert = &sm.vertices[j];
            pVert->position.x = shape_mesh.positions[j*3];
            pVert->position.y = shape_mesh.positions[j*3+1];
            pVert->position.z = shape_mesh.positions[j*3+2];

            pVert->tex_coords.x = shape_mesh.texcoords[j*2];
            pVert->tex_coords.y = shape_mesh.texcoords[j*2+1];

            //if(shape_mesh.normals.size()>0)
            //{
                pVert->normal.x = 0.0f;
                pVert->normal.y = 0.0f;
                pVert->normal.z = 1.0f;
            //}
        }

        m_mesh->AddSubMesh(sm);
    }
}