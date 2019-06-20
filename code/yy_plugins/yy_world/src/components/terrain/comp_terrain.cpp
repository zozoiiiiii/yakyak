#include "comp_terrain.h"
#include "texture/stbi_DDS_aug.h"

#define MAP_SCALE 0.1        // 地图缩放
#define MAP_HEIGHT 1        // 地图最大高差

void Comp_Terrain::OnCreate()
{
}

void Comp_Terrain::OnDestroy()
{
}


void Comp_Terrain::LoadTex(const std::string& strHighMap, const std::string& strTex)
{
    throw_assert(NULL==m_mesh, "null check.");
    m_mesh = IRender::Instance(GetMgr())->GetModelResMgr()->CreateMesh();

    // create mesh
    YY::SubMesh sm;


    std::string res_path = IRender::Instance(GetMgr())->GetResPath();

    int width,height,n;
    std::string path = res_path + strHighMap;
    unsigned char* pData = stbi_load(path.c_str(), &width, &height, &n, 0);
    throw_assert(NULL!=pData, "read image failed." << strHighMap);
    for(int i =0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            YY::Vertex v;
            v.position.x =j*MAP_SCALE- height/2*MAP_SCALE;
            // 先暂时地面平整。
            v.position.y = pData[(i*width+j)*3]*1.0/255*MAP_HEIGHT -0.5*MAP_HEIGHT;
            v.position.z = i*MAP_SCALE - width/2*MAP_SCALE;

            v.tex_coords.x = j*1.0/width;
            v.tex_coords.y = i*1.0/height;
            sm.vertices.push_back (v);
        }
    }

    for(int i =0;i<width-1;i++)
    {
        for(int j = 0;j<height-1;j++)
        {
            sm.indices.push_back(i*width+j);
            sm.indices.push_back((i+1)*width+j);
            sm.indices.push_back(i*width+j+1);

            sm.indices.push_back(i*width+j+1);
            sm.indices.push_back((i+1)*width+j);
            sm.indices.push_back((i+1)*width+j+1);
        }
    }


    m_mesh->CalNormalLines(&sm);

    //sm.WireFrame=true;
    sm.material.diffuse = strTex;
    m_mesh->AddSubMesh(sm);

}