#include "model_res.h"
#include "mesh.h"
#include "anim.h"
#include "skeleton.h"
#include "yy_render/inc/i_render.h"


ModelRes* ModelRes::Instance()
{
    static ModelRes s_model_res;
    return &s_model_res;
}

YY::IMesh* ModelRes::CreateMesh()
{
    YY::Mesh* pMesh = new YY::Mesh;
    return pMesh;
}

YY::IMesh* ModelRes::LoadMeshFile(const std::string& path, const std::string& file)
{
	YY::Mesh* pMesh = new YY::Mesh;
	pMesh->Load(path, file);
	return pMesh;
}

YY::Skeleton* ModelRes::LoadSkelFile(const std::string& file)
{
	YY::Skeleton* pSkel = new YY::Skeleton;
	LoadSkeleton(pSkel, file);
	return pSkel;
}

YY::IAnim* ModelRes::LoadAnimFile(YY::Skeleton* skel, const std::string& file)
{
	YY::Anim* pAnim = new YY::Anim;
	pAnim->Load(skel, file);
	return pAnim;
}