#include "comp_model_assimp.h"
#include "texture/cg_texture.h"
#include "texture/stb_image_aug.h"
#include "cg.h"
#include "render/mesh.h"

#include "assimp/inc/scene.h"
#include "assimp/inc/Importer.hpp"
#include "assimp/inc/postprocess.h"


YY_DECLARE_CLASS(Comp_Model_Assimp, Component);
void Comp_Model_Assimp::OnCreate(const YY::VarList& args)
{
}

void Comp_Model_Assimp::OnDestroy()
{
}


void Comp_Model_Assimp::InitModel(const std::string& strModelFile, const std::string& strModelDir)
{
    throw_assert(NULL==m_mesh, "null check.");
    m_mesh = new YY::Mesh;


    Assimp::Importer m_Importer;
    const aiScene* pScene = m_Importer.ReadFile(strModelFile,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |aiProcess_CalcTangentSpace);


    //m_globalInverseTransform = pScene->mRootNode->mTransformation;
    //m_globalInverseTransform = m_globalInverseTransform.Inverse();

    //char str[100]={'\0'};
    //utility::FindPrefix(strModelFile,str);

    std::string strDir = YY::GetPrePath(strModelFile);
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for(int i =0 ;i< pScene->mNumMeshes ;i++)
    {
        YY::SubMesh sm;

        const aiMesh* the_mesh = pScene->mMeshes[i];
        int nMatIndex = the_mesh->mMaterialIndex;
        aiMaterial * the_material = pScene->mMaterials[nMatIndex];

        aiString diffuse_Path;
        the_material->GetTexture(aiTextureType_DIFFUSE,0,&diffuse_Path);

        if(diffuse_Path.length<=0)
            sm.material.diffuse = "";
        else
            sm.material.diffuse = strModelDir + diffuse_Path.C_Str();

        for(int j =0; j<the_mesh->mNumVertices;j++)
        {
            const aiVector3D* pPos = &(the_mesh->mVertices[j]);
            const aiVector3D* pNormal = &(the_mesh->mNormals[j]);
            const aiVector3D* pTexCoord = the_mesh->HasTextureCoords(0) ? &(the_mesh->mTextureCoords[0][j]) : &Zero3D;
            const aiVector3D* pTangent = &(the_mesh->mTangents[i]);
            YY::Vertex vec;
            vec.position = YY::Vec3f(pPos->x,pPos->y,pPos->z);
            vec.normal = YY::Vec3f(pNormal->x,pNormal->y,pNormal->z);
            vec.tex_coords = YY::Vec2f(pTexCoord->x,pTexCoord->y);
            sm.vertices.push_back(vec);
        }

        for (unsigned int k = 0 ; k < the_mesh->mNumFaces ; k++) {
            const aiFace& Face = the_mesh->mFaces[k];
            assert(Face.mNumIndices == 3);
            sm.indices.push_back(Face.mIndices[0]);
            sm.indices.push_back(Face.mIndices[1]);
            sm.indices.push_back(Face.mIndices[2]);
        }

        //load bones, cancel.

        m_mesh->AddSubMesh(sm);
    }
}

