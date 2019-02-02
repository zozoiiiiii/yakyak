#include "comp_model_obj.h"
#include "tiny_obj_loader.h"

#define MAP_SCALE 0.1        // 地图缩放
#define MAP_HEIGHT 1        // 地图最大高差

void Comp_Model_Obj::OnCreate()
{
//     std::string model_name = args.GetStr(0);
//     std::string model_dir = args.GetStr(1);
//     InitModel(model_name, model_dir);
}

void Comp_Model_Obj::OnDestroy()
{
}


void Comp_Model_Obj::InitModel(const std::string& strModelFile, const std::string& strModelDir)
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
    //throw_assert(nSubMeshNum == nMatNum, "submesh num should equal mat num.");

    for(int i=0; i<m_file_shapes.size(); i++)
    {
        tinyobj::mesh_t& shape_mesh=m_file_shapes[i].mesh;
        for(int j=0; j<shape_mesh.positions.size(); j++)
        {
            //shape_mesh.positions[j]=shape_mesh.positions[j]/2;
        }

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

            if(shape_mesh.normals.size()>0)
            {
                pVert->normal.x = shape_mesh.normals[j*3];
                pVert->normal.y = shape_mesh.normals[j*3+1];
                pVert->normal.z = shape_mesh.normals[j*3+2];
            }
        }

        m_mesh->AddSubMesh(sm);
    }

    for(int i=0; i<m_file_materials.size(); i++)
    {
        const tinyobj::material_t* pTM=&m_file_materials[i];
        std::string diffuseTexName=strModelDir+pTM->diffuse_texname;

        YY::SubMesh* pSM = m_mesh->GetSubMesh(i);
        pSM->material.diffuse = diffuseTexName;
    }



    // 此处是导入的一个立方体， 属于sharp mesh；而我参考自tangziwen的引擎里的CaculateNormalLine时，
    // 地形，天空盒他是自己计算的。立方体是通过assimp来计算出来的。
    // 综上， sharp meshes normals and smooth meshes normals做法不同。
    // smooth meshes可以通过下面的方式，顶点所在平面的所有法线的平均值来获得。而sharp mesh则不可以。
    // generate normal line
    for(int j=0; j<m_file_materials.size(); j++)
    {
        YY::SubMesh* pSM = m_mesh->GetSubMesh(j);

        int indices_count = pSM->indices.size();
        for (unsigned int i = 0 ; i < indices_count ; i += 3) {

            // find a triangle
            unsigned int Index0 = pSM->indices[i];
            unsigned int Index1 = pSM->indices[i + 1];
            unsigned int Index2 = pSM->indices[i + 2];
            YY::Vertex* v0 = &pSM->vertices[Index0];
            YY::Vertex* v1 = &pSM->vertices[Index1];
            YY::Vertex* v2 = &pSM->vertices[Index2];

            // two vector in this triangle
            YY::Vec3f v10 = v1->position - v0->position;
            YY::Vec3f v20 = v2->position - v0->position;

            // normal line = vector1 corss vector2
            YY::Vec3f Normal = v10.cross(v20);
            //YY::Vec3f Normal = v20.cross(v10);
            //QVector3D::crossProduct(v1,v2);

            // this normal line depends on this two vectors and the angle, so must normalize.
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
}