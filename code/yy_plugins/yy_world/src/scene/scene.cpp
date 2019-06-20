#include "scene.h"
#include "../world.h"
#include "yy_world/inc/components/i_comp_camera.h"
#include "yy_world/inc/components/i_comp_mesh_render.h"
#include "yy_world/inc/components/i_show_depth_map.h"
#include <algorithm>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

//#define FREEIMAGE_LIB
//#include "freeimage/include/freeimage.h"


void Scene::OnCreate(const VariantMap& args)
{
    //FreeImage_Initialise();
	m_pMainCamera = nullptr;
	IWorld* pWorld = IWorld::Instance(GetMgr());
	pWorld->SetSceneID(this->GetID());

    m_pBatchGroup = (IBatchGroup*)GetMgr()->Create("BatchGroup");
    m_pCurRenderText = m_pBatchGroup->GetRenderContext();
    m_pCurRenderText->nRenderType = RT_Normal;

    // color texture
    IRenderTexture* pRenderTexture = (IRenderTexture*)GetRender()->GetResMgr()->CreateRes("ColorTexture");
    pRenderTexture->SetFormat(TEX_FORMAT_rgb_alpha);
    pRenderTexture->SetSizeType(TEX_TYPE_DEVICE);
    pRenderTexture->Generate();
    m_pCurRenderText->pRenderTexture = pRenderTexture;


    // this depth buffer is used for depth test.  depth render buffer is faster than depth texture, and no need to sample.
    IRenderBuffer* pRenderBuffer = (IRenderBuffer*)GetRender()->GetResMgr()->CreateRes("DepthRenderBuffer");
    pRenderBuffer->SetFormat(TEX_FORMAT_DEPTH_DEFAULT);
    pRenderBuffer->SetSizeType(TEX_TYPE_DEVICE);
    pRenderBuffer->Generate();
    m_pCurRenderText->pDepthRenderBuffer = pRenderBuffer;

    // offscreen frame buffer
    IFrameBuffer* pFBO =  (IFrameBuffer*)GetRender()->GetResMgr()->CreateRes("FrameBuffer");
    pFBO->Open();
    pFBO->AttachTexture(pRenderTexture, AT_COLOR);
    pFBO->AttackRenderBuffer(pRenderBuffer, AT_DEPTH);
    m_pCurRenderText->pFBO = pFBO;

    m_pWeather = (Weather*)GetMgr()->Create("Weather");
    m_pShadowMap = (ShadowMap*)GetMgr()->Create("ShadowMap");
    m_pShadowMap->EnableShadow(true);

    AABB sceneBounding;
    sceneBounding.update(Vec3f(-2000,-200,-2000));
    sceneBounding.update(Vec3f(2000,200,2000));
    m_pOctreeScene = (OctreeScene*)GetMgr()->Create("OctreeScene");
    m_pOctreeScene->init(sceneBounding);

     IEventMgr* pEventMgr = IWorld::Instance(GetMgr())->GetEventMgr();
     pEventMgr->ConnectGlobals("OnEvent_RenderWindow_Resize",  fastdelegate::MakeDelegate(this, &Scene::OnEvent_Resize));
}

void Scene::OnDestroy()
{
//    FreeImage_DeInitialise();
}

void Scene::Update(float sec)
{
    m_pOctreeScene->cullingByCamera(GetMainCamera());
    std::vector<YY_OBJECTID>& visibleList = m_pOctreeScene->getVisibleList();

    int nSize = visibleList.size();
    for(int i=0; i<nSize; i++)
    {
        YY_OBJECTID id = visibleList[i];
        YY::BaseObject* pBaseObject = GetMgr()->Get(id);
        bool bRet = pBaseObject->IsInstanceOf("GameObj");
        throw_assert(bRet, "type check.");
        IGameObj* pGameObj = (IGameObj*)pBaseObject;
        pGameObj->OnUpdate(sec);
    }
}


RenderContext* Scene::GetRenderCxt()
{
    return m_pCurRenderText;
}

IRender* Scene::GetRender()
{
	return IRender::Instance(GetMgr());
}

IGameObj* Scene::CreateObj(IGameObj* pParent)
{
    IGameObj* pGameObj = (IGameObj*)GetMgr()->Create("GameObj");
    pGameObj->SetParent(pParent);
    pGameObj->SetScene(this);

    if(pParent)
    {
        pParent->AddChild(pGameObj);
    }

    return pGameObj;
}

bool Scene::AddObj(IGameObj* pObj)
{
    if(NULL == pObj)
        return false;

    m_pOctreeScene->addObj(pObj);
    return true;
}

bool Scene::RemoveObj(IGameObj* pObj)
{
    if(NULL == pObj)
        return false;

    m_pOctreeScene->removeObj(pObj);
    return false;
}


void Scene::UpdateRenderContext()
{
    IGameObj* pCamera = GetMainCamera();
    IComp_Camera* pComp_Camera = (IComp_Camera*)pCamera->FindComponent("Comp_Camera");
    m_pCurRenderText->projMatrix = pComp_Camera->GetPersp();

    // view matrix is camera tm inverse.
    YY::Mat4f cameraTM;
    pCamera->GetTransform()->GetTMMatrix(&cameraTM);
    cameraTM.Inverse();
    m_pCurRenderText->viewMatrix = cameraTM;
}

void Scene::VisitAllObjs()
{
    // 1. 裁剪获得可视对象集合
    m_pOctreeScene->cullingByCamera(GetMainCamera());
    m_curRendingGameObjs = m_pOctreeScene->getVisibleList();

	// 遍历模型，drawcall插入批处理组；
	int nSize = GetVisibleListNum();
	for(int i=0; i<nSize; i++)
	{
		YY_OBJECTID id = GetVisibleObjByID(i);
		YY::BaseObject* pBaseObject = GetMgr()->Get(id);
		bool bRet = pBaseObject->IsInstanceOf("GameObj");
		throw_assert(bRet, "type check.");
		IGameObj* pGameObj = (IGameObj*)pBaseObject;

		//pGameObj->OnAddRender(m_pBatchGroup);
		// components
		std::vector<Component*> components = pGameObj->GetAllComponents();
		int nCnt = components.size();
		for (int j = 0; j < nCnt; j++)
		{
			Component* pComponent = components[j];
			if (!pComponent->IsInstanceOf("RenderComponent"))
				continue;

			RenderComponent* pRenderComponent = (RenderComponent*)pComponent;
			pRenderComponent->OnAddBatch(m_pBatchGroup);
		}
	}
}

void Scene::Render()
{
    //glEnable(GL_DEPTH_TEST);

    VisitAllObjs();

    // shadow map pass -> depth map rt
    m_pShadowMap->GenerateShadowMap(m_curRendingGameObjs, m_pCurRenderText);

    ITexture* pDepthTexture = m_pShadowMap->GetDepthTexture();
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pDepthTexture->GetID());

    UpdateRenderContext();

    GetRender()->AddGroup(m_pBatchGroup);
}

IGameObj* Scene::FindObjByName(const std::string& name)
{
    return m_pOctreeScene->FindObjByName(name);
}


IGameObj* Scene::HitByRay(const YY::Ray &ray)
{
	// 通过八叉树来实现射线拾取功能
	YY::Vec3f hit_point;
	return m_pOctreeScene->hitByRay(ray, hit_point);



//     m_pOctreeScene->cullingByCamera(m_camera);
//     std::vector<YY_OBJECTID>& visibleList = m_pOctreeScene->getVisibleList();
// 
//     int nSize = visibleList.size();
//     for(int i=0; i<nSize; i++)
//     {
//         YY_OBJECTID id = visibleList[i];
//         YY::BaseObject* pBaseObject = GetMgr()->Get(id);
//         bool bRet = GetMgr()->IsInstanceOf(pBaseObject, "GameObj");
//         throw_assert(bRet, "type check.");
//         IGameObj* pGameObj = (IGameObj*)pBaseObject;
// 
//         std::string name = pGameObj->GetName();
//         YY::AABB aabb = pGameObj->GetAABB_World();
//         bool ret = ray.intersectAABB(aabb,NULL,hit_point);
//         if(ret)
//             return pGameObj;
//     }
// 
// 
//     return NULL;
}


int Scene::GetVisibleListNum()
{
	return m_curRendingGameObjs.size();
}

YY_OBJECTID Scene::GetVisibleObjByID(int index)
{
	throw_assert(index >= 0 && index <= GetVisibleListNum(), "index check."<<index);
	return m_curRendingGameObjs[index];
}

int Scene::GetObjListNum()
{
    return m_pOctreeScene->getAllObjList().size();
}

YY_OBJECTID Scene::GetObjByID(int index)
{
    const std::vector<YY_OBJECTID>& objs = m_pOctreeScene->getAllObjList();
    throw_assert(index >= 0 && index <= objs.size(), "index check." << index);
    return objs[index];
}

void Scene::OnEvent_Resize(const char* name, const YY::VarList& args)
{
    int w = args.GetInt(0);
    int h = args.GetInt(1);

    // 如果是窗口大小变化导致的渲染区域变化，则需要更新fbo.
    //bool bWindowSizeChange = args.GetBool(2);
    //if(bWindowSizeChange)
    {
        //m_pShadowMap->GetRT()->SetSize(w,h);
        //m_offScreenBuffer->SetSize(w,h);
        GetRender()->SetWinWidth(w);
        GetRender()->SetWinHeight(h);
        GetRender()->SetDeviceWidth(w);
        GetRender()->SetDeviceHeight(h);
        GetRender()->ResetDevice();

		// 后期做再resetdevice函数里
		ITexture* pDepthTexture = m_pShadowMap->GetDepthTexture();
		pDepthTexture->Generate();
        IFrameBuffer* pFrameBuffer = m_pShadowMap->GetFrameBuffer();
        pFrameBuffer->AttachTexture(pDepthTexture, AT_DEPTH);

        m_pCurRenderText->pDepthRenderBuffer->Generate();
        m_pCurRenderText->pFBO->AttackRenderBuffer(m_pCurRenderText->pDepthRenderBuffer, AT_DEPTH);

        m_pCurRenderText->pRenderTexture->Generate();
        m_pCurRenderText->pFBO->AttachTexture(m_pCurRenderText->pRenderTexture, AT_COLOR);
    }
}



//
//bool RICompare( RenderInfo* pfirst, RenderInfo* psecond)
//{
//    return pfirst->nPriority < psecond->nPriority;
//}
//
//void Scene::AddRender(RenderInfo* pRI)
//{
//    m_pRenderInfos.push_back(pRI);
//
//    std::sort(m_pRenderInfos.begin (),m_pRenderInfos.end (),RICompare);
//}