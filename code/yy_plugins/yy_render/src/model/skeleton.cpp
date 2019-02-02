#include "skeleton.h"
#include "model_res.h"
#include "core/inc/yy_file.h"

NS_YY_BEGIN
void InitBone(Bone* pBone)
{
    pBone->parent=-1;
    pBone->angle=pBone->ax=pBone->ay=pBone->az=0.0f;
}


void LoadSkeleton(Skeleton* pSkeleton, const std::string& file)
{
    std::string res_path = ModelRes::Instance()->GetResPath();
    std::string path = res_path + file;
    FILE *infile = fopen(path.c_str(), "rt");
    throw_assert (infile, "open file faile:"<<file);

    int numbones;
    fscanf(infile, "%d\n", &numbones);
    if (numbones)
    {
        pSkeleton->bones.resize(numbones);
    }

    char buff[200];
    for (int i = 0; i < numbones; i++)
    {
        Bone& jt = pSkeleton->bones[i];

        fscanf(infile, "%s %d (%f %f %f) (%f %f %f %f) (%f %f %f)\n",
            &buff, &jt.parent, &jt.pos.x, &jt.pos.y, &jt.pos.z,
            &jt.angle, &jt.ax, &jt.ay, &jt.az,
            &jt.scale.x, &jt.scale.y, &jt.scale.z);

        jt.name = buff;

        if(jt.parent != -1)
        {
            Bone& jt_parent = pSkeleton->bones[jt.parent];
            jt_parent.children.push_back(i);
        }
    }

    fclose(infile);
}

NS_YY_END