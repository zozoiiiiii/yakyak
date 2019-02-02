#include "md_writer.h"
#include "yy_core/util/inc/yy_file.h"
#include "yy_core/entity/inc/yy_exception.h"
#include "yy_core/util/inc/yy_string.h"

void MDMeshWrite(const std::string& file, MD_Mesh* pMesh)
{
    FILE *infile = fopen(file.c_str(), "w");
    throw_assert(infile!=NULL, "open file faile:"<<file);

    int nSubMeshNum = pMesh->subMeshes.size();
    fprintf(infile, "submesh num: %d\n", nSubMeshNum);

    for (int i=0; i<nSubMeshNum; i++)
    {
        const MD_SubMesh& sm = pMesh->subMeshes[i];

        fprintf(infile, "this is sub mesh:%d, has anim: %d\n", i, sm.hasAnim);
        fprintf(infile, "( diffuse:%d %s )( opacity:%d %s )\n", sm.material.diffuse.length(), sm.material.diffuse.c_str(),
            sm.material.opacity.length(), sm.material.opacity.c_str());
        //fprintf(infile, "specular:%d %s\n", sm.material.specular.length(), sm.material.specular.c_str());
        //fprintf(infile, "glossiness:%d %s\n", sm.material.glossiness.length(), sm.material.glossiness.c_str());
        //fprintf(infile, "glossiness:%d %s\n", sm.material.glossiness.length(), sm.material.glossiness.c_str());
        //fprintf(infile, "specularLevel:%d %s\n", sm.material.specularLevel.length(), sm.material.specularLevel.c_str());

        int vt_num = sm.vertices.size();
        fprintf(infile, "vertex num: %d\n", vt_num);
        for(int i=0; i<vt_num; i++)
        {
            int nIndex = i;
            const MD_Vertex& vt = sm.vertices[i];

            fprintf(infile, "%d (%f %f %f) (%f %f %f) (%f %f) (%f %f %f %f) (%d %d %d %d)\n",
                i,
                vt.x,vt.y,vt.z,
                vt.nx, vt.ny, vt.nz,
                vt.u,vt.v,
                vt.bone_weight[0],
                vt.bone_weight[1],
                vt.bone_weight[2],
                vt.bone_weight[3],
                vt.bone_id[0],
                vt.bone_id[1],
                vt.bone_id[2],
                vt.bone_id[3]);
        }

        int face_num = sm.faces.size();
        fprintf(infile, "face num: %d\n", face_num);
        for (int iFace=0; iFace<face_num; iFace++)
        {
            const MD_Face& fe = sm.faces[iFace];

            int index_num = 3;
            fprintf(infile, "%d %d ", iFace, index_num);
            for (unsigned int i = 0; i < index_num; i++)
            {
                fprintf(infile, "%d ", fe.indices[i]);
            }
            fprintf(infile, "\n");
        }
    }

    fclose(infile);
}


void MDSkeletonWrite(const std::string& file, MD_Skeleton* pSkel)
{
    FILE *infile = fopen(file.c_str(), "w");
    throw_assert(infile != NULL, "open file faile:"<<file);

    int bone_num = pSkel->bones.size();
    fprintf(infile, "%d\n", bone_num);
    for(int i=0; i<bone_num; i++)
    {
        const MD_Bone& bi = pSkel->bones[i];


        // È¥µô¿Õ¸ñ
        std::string name = bi.name;
        YY::StrReplace(name, " ", "_");
        fprintf(infile, "%s %d (%f %f %f) (%f %f %f %f) (%f %f %f)\n",
            name.c_str(), bi.parent,
            bi.x,bi.y,bi.z,
            bi.angle,bi.ax,bi.ay,bi.az,
            bi.sx,bi.sy,bi.sz);
    }
    fclose(infile);
}

void MDAnimWrite(const std::string& file, MD_Anim* pAnim)
{
    FILE *infile = fopen(file.c_str(), "w");
    throw_assert(infile != NULL, "open file faile:"<<file);

    fprintf(infile, "time len:%f\n", pAnim->time_length);
    fprintf(infile, "frame num:%d\n", pAnim->frame_num);

    int bone_num = pAnim->tracks.size();
    fprintf(infile, "bone num:%d\n", bone_num);
    for(int i=0; i<bone_num; i++)
    {
        const MD_Track& tk = pAnim->tracks[i];
        int kf_num = tk.keys.size();
        fprintf(infile, "keyframe num: %d\n", kf_num);
        for(int j=0; j<kf_num; j++)
        {
            const MD_KeyFrame& kf = tk.keys[j];
            fprintf(infile, "%f (%f %f %f) (%f %f %f %f) (%f %f %f)\n",
                kf.time,
                kf.x,kf.y,kf.z,
                kf.angle,kf.ax,kf.ay,kf.az,
                kf.sx,kf.sy,kf.sz);
        }
    }
    fclose(infile);
}