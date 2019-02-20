/************************************************************************/
/* 
@author:    junliang
@brief:     mesh render
@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "yy_render/inc/model/i_mesh.h"
#include "yy_render/inc/model/i_skeleton.h"
#include "yy_render/inc/model/i_anim.h"
#include <vector>
#include "yy_core.h"
#include "../../components/camera/comp_camera.h"
#include "yy_world/inc/i_game_obj.h"
#include "yy_world/inc/components/i_comp_mesh_render.h"
#include "yy_render/inc/i_render.h"

class Comp_MeshRender : public IComp_MeshRender
{
    YY_BEGIN(Comp_MeshRender, IComp_MeshRender);
	YY_METHOD(&Comp_MeshRender::SetMesh, "SetMesh", "");
	YY_FIELD(&Comp_MeshRender::m_path, "path", "");
	YY_FIELD(&Comp_MeshRender::m_mesh_file, "mesh_file", "");
	YY_FIELD(&Comp_MeshRender::m_skeleton_file, "skeleton_file", "");
    YY_END
public:
    Comp_MeshRender():m_mesh(NULL),m_skele(NULL){}
    ~Comp_MeshRender(){}
    virtual void OnCreate(const VariantMap& args);
    virtual void OnDestroy();
	virtual void OnRender(IRender* pRender, RenderContext* pCxt);
	//virtual void OnRender(RenderContext* pCxt);

    virtual void SetMesh(const std::string& path, const std::string& mesh_file, const std::string& Skeleton_file=std::string());
    virtual int AddAnim(const std::string& anim_file, float fEnterTime=0.0f, float fLeaveTime=0.0f);
    virtual void SetDefaultAnim(int nAnimIndex);
    virtual void Blend(int nAnimIndex);
protected:
    int m_active_anim;
    std::string m_path;
	std::string m_mesh_file;
	std::string m_skeleton_file;
	std::vector<std::string> m_anims_name;
	
    YY::IMesh* m_mesh;
    YY::Skeleton* m_skele;
    std::vector<YY::IAnim*> m_anims;

    // ÈÚºÏÁÐ±í
    std::vector<int> m_blendAnims;
};