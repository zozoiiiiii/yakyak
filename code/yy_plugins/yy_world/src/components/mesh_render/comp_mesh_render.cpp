#include "comp_mesh_render.h"
#include "tinyxml/tinyxml.h"
#include "yy_render/inc/i_render.h"
#include "yy_world/inc/i_world.h"


void Comp_MeshRender::OnCreate()
{}

void Comp_MeshRender::OnDestroy()
{
}

void Comp_MeshRender::OnRender(RenderContext* pCxt)
{
    std::string vsh = IRender::Instance()->GetResPath() + "shader/mesh.vsh";
    std::string fsh = IRender::Instance()->GetResPath() + "shader/mesh.fsh";
    IShader* pMeshShader = IRender::Instance()->GetResMgr()->LoadShader(vsh, fsh);

    // 如果是渲染阴影，则用对应的shader
    IShader* pShader = pMeshShader;
    if(pCxt->nRenderType == RT_Shadow)
        pShader = pCxt->pShader_ShadowMap;

    //告诉gpu一次要执行的命令
    pShader->Begin();


    if(pCxt->nRenderType == RT_Normal)
    {
        YY::Vec3f color = pCxt->ambient_color;
        pShader->SetUniform3f("ambient.color", color.x, color.y, color.z);
        pShader->SetUniform1f("ambient.intensity", pCxt->ambient_intensity);

        // direction light
        color = pCxt->directionLight_color;
        pShader->SetUniform3f("directionLight.color", color.x, color.y, color.z);
        pShader->SetUniform1f("directionLight.intensity", pCxt->directionLight_intensity);
        YY::Vec3f direction = pCxt->directionLight_direction;
        pShader->SetUniform3f("directionLight.direction", direction.x, direction.y, direction.z);

        // LightVPMatrix
        YY::Mat4f lightView;
        lightView.LookAtRh(direction.norm(), YY::Vec3f(0,1,0));
        lightView.Inverse();

        YY::Mat4f lightProj;
        lightProj.OrthoRh(-25,25,-25,25,0.1f,20.0f);
        YY::Mat4f lightPV = lightProj * lightView;
        pShader->SetUniformMat4fv("LightVPMatrix", 1, lightPV.m_data);



        // 使用深度图的纹理，对应前面BindForReading的纹理索引（考虑放到scene类里面）
        pShader->SetUniform1i("ShadowMap", 2);
    }

    //----------
    throw_assert(NULL!=m_mesh, "null check.");

	IGameObj* pGameObj = FindGameObj();
	if (!pGameObj)
		return;

    YY::Mat4f model;
    pGameObj->GetTransform()->GetTMMatrix(&model);

    //YY::Mat4f mvp = pCxt->projM * pCxt->viewM * model;



    // matrix
    //pShader->SetUniformMat4fv("u_mvp", 1, &mvp.m_data );
    pShader->SetUniformMat4fv("u_view", 1, pCxt->viewMatrix.m_data );
    pShader->SetUniformMat4fv("u_proj", 1, pCxt->projMatrix.m_data );
    pShader->SetUniformMat4fv("u_model", 1, model.m_data );

    if(m_anims.size() > 0 && m_active_anim >=0)
    {
        //移动骨骼
        YY::IAnim* active_anim = m_anims[m_active_anim];
        active_anim->render(pShader);
    }

    m_mesh->draw(pShader);

    // 告诉gpu结束该命令
    pShader->End();
}


bool Comp_MeshRender::SerializeTo(YY::VarList& args)
{
//	if(!YY::BaseObject::SerializeTo(args))
		return false;

	int anim_cnt = m_anims_name.size();
	args<<anim_cnt;
	for(int i=0; i<anim_cnt; i++)
	{
		args<<m_anims_name[i];
	}

    return true;
}

bool Comp_MeshRender::ParseFrom(const YY::VarList& args, int& read_index)
{
//	if(!YY::BaseObject::ParseFrom(args, read_index))
		return false;

	SetMesh(m_path, m_mesh_file, m_skeleton_file);

	int args_cnt = args.GetCount();
	YY::VarList anim_args;
	anim_args.Append(args, read_index, args_cnt-read_index);
	int anim_cnt = anim_args.GetInt(0);
	for(int i=0; i<anim_cnt; i++)
	{
		std::string anim_name = anim_args.GetStr(i+1);
		int nAnim1=AddAnim(anim_name);
		if(i==0)
			SetDefaultAnim(nAnim1);
	}

	return true;
}

void Comp_MeshRender::SetMesh(const std::string& path, const std::string& mesh_file, const std::string& Skeleton_file)
{
    throw_assert(NULL == m_mesh, "only support one mesh.");

    IModelRes* pModelRes = IRender::Instance()->GetModelResMgr();

    //m_path = YY::SlashTrim(path, true);
	m_path = path;
	m_mesh_file = mesh_file;
	m_skeleton_file = Skeleton_file;

    m_mesh = pModelRes->LoadMeshFile(m_path, mesh_file);
    if(!Skeleton_file.empty())
    {
        m_skele=pModelRes->LoadSkelFile(m_path+Skeleton_file);
    }

    m_active_anim = -1;

    // aabb
	IGameObj* pGameObj = FindGameObj();
	if(pGameObj)
		pGameObj->SetAABB(m_mesh->GetAABB());
}

int Comp_MeshRender::AddAnim(const std::string& anim_file, float fEnterTime, float fLeaveTime)
{
    IModelRes* pModelRes = IRender::Instance()->GetModelResMgr();
    YY::IAnim* am =  pModelRes->LoadAnimFile(m_skele, m_path + anim_file);
    m_anims.push_back(am);
	m_anims_name.push_back(anim_file);
    return m_anims.size()-1;
}

void Comp_MeshRender::SetDefaultAnim(int nAnimIndex)
{
    int nTotalAnimNum = m_anims.size();
    throw_assert(nAnimIndex>=0&&nAnimIndex<=nTotalAnimNum, "animation index check."<<nTotalAnimNum<<nAnimIndex);
    m_active_anim = nAnimIndex;
}

void Comp_MeshRender::Blend(int nAnimIndex)
{
    m_active_anim=nAnimIndex;
}
