#include "shadow_map.h"
#include "scene.h"
#include "yy_world/inc/i_world.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

void ShadowMap::OnCreate(const VariantMap& args)
{
    // disable shadow default.
    m_bEnableShadow = false;
	Scene* pScene = (Scene*)IWorld::Instance()->GetScene();
    m_pRender = pScene->GetRender();


    m_pDepthTexture = (ITexture*)m_pRender->GetResMgr()->CreateRes("DepthTexture");
	m_pDepthTexture->SetFormat(TEX_FORMAT_DEPTH_DEFAULT);
    m_pDepthTexture->SetSizeType(TEX_TYPE_DEVICE);
    m_pDepthTexture->Generate();

    m_pFrameBuffer = (IFrameBuffer*)m_pRender->GetResMgr()->CreateRes("FrameBuffer");
    m_pFrameBuffer->Open();
	m_pFrameBuffer->AttachTexture(m_pDepthTexture, AT_DEPTH);

}

void ShadowMap::GenerateShadowMap(std::vector<YY_OBJECTID>& objs, RenderContext* pRenderContext)
{
    if(!m_bEnableShadow)
        return;

	RenderContext tmp = *pRenderContext;

    m_pFrameBuffer->BindForWriting();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	static IShader* pShader = NULL;
	if(NULL == pShader)
	{
		std::string vsh = m_pRender->GetResPath() + "shader/shadow_map.vsh";
		std::string fsh = m_pRender->GetResPath() + "shader/shadow_map.fsh";
		pShader = m_pRender->GetResMgr()->LoadShader(vsh, fsh);
	}
	tmp.pShader_ShadowMap = pShader;
	tmp.nRenderType = RT_Shadow;

    // render game obj
    int nSize = objs.size();
    for(int i=0; i<nSize; i++)
    {
        YY_OBJECTID id = objs[i];
        YY::BaseObject* pBaseObject = GetMgr()->Get(id);
        bool bRet = pBaseObject->IsInstanceOf("GameObj");
        throw_assert(bRet, "type check.");
        IGameObj* pGameObj = (IGameObj*)pBaseObject;
        if(!pGameObj->GetReceiveShader())
            continue;

        YY::Vec3f direction_light = tmp.directionLight_direction;

        // 在光源位置作为摄像机位置，计算视图矩阵
        YY::Mat4f viewM;
        viewM.LookAtRh(direction_light.norm(), YY::Vec3f(0,1,0));
        viewM.Inverse();
        tmp.viewMatrix = viewM;

        // 直射光，用正交矩阵
        YY::Mat4f projM;
        projM.OrthoRh(-25,25,-25,25,0.1f,20.0f);
        tmp.projMatrix = projM;

		//pGameObj->OnRender(&tmp);
		std::vector<Component*> components = pGameObj->GetAllComponents();
		int nCnt = components.size();
		for (int j = 0; j < nCnt; j++)
		{
			Component* pComponent = components[j];
			if (!pComponent->IsInstanceOf("RenderComponent"))
				continue;

			RenderComponent* pRenderComponent = (RenderComponent*)pComponent;
			pRenderComponent->OnRender(IRender::Instance(), &tmp);
		}
    }
}