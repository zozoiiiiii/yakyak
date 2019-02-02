#include "gui.h"
//#include "font/font_manager.h"

void GUI::OnCreate(const VariantMap& args)
{
	//FontManager::Instance()->Init(GetMgr());

	m_pRender = IRender::Instance();
	m_pUIBatchGroup = m_pRender->GetUIBatchGroup();
	m_pRenderContext = m_pUIBatchGroup->GetRenderContext();
	m_pDeskTop = (Entity*)GetMgr()->Create("Entity");
	if (nullptr == m_pDeskTop)
		return;

	Component* pComponent = m_pDeskTop->AddComponent("ItemTransformComponent");
	if (!pComponent)
	{
		GetMgr()->Destroy(m_pDeskTop->GetID());
		return;
	}

	m_pDeskTop->SetName("DeskTopForm");

	GetEventMgr()->ConnectGlobals("AddBatch_Items", fastdelegate::MakeDelegate(this, &GUI::onEvent_AddBatch));


	m_pRenderContext->nRenderType = RT_Normal;

	// color texture
	IRenderTexture* pRenderTexture = (IRenderTexture*)m_pRender->GetResMgr()->CreateRes("ColorTexture");
	pRenderTexture->SetFormat(TEX_FORMAT_rgb_alpha);
	pRenderTexture->SetSizeType(TEX_TYPE_DEVICE);
	pRenderTexture->Generate();
	m_pRenderContext->pRenderTexture = pRenderTexture;


	IRenderBuffer* pRenderBuffer = (IRenderBuffer*)m_pRender->GetResMgr()->CreateRes("DepthRenderBuffer");
	pRenderBuffer->SetFormat(TEX_FORMAT_DEPTH_DEFAULT);
	pRenderBuffer->SetSizeType(TEX_TYPE_DEVICE);
	pRenderBuffer->Generate();
	m_pRenderContext->pDepthRenderBuffer = pRenderBuffer;

	// offscreen frame buffer
	IFrameBuffer* pFBO = (IFrameBuffer*)m_pRender->GetResMgr()->CreateRes("FrameBuffer");
	pFBO->Open();
	pFBO->AttachTexture(pRenderTexture, AT_COLOR);
	pFBO->AttackRenderBuffer(pRenderBuffer, AT_DEPTH);
	m_pRenderContext->pFBO = pFBO;
}

void GUI::OnDestroy()
{

}

void GUI::ReSize(float w, float h)
{
	if (nullptr == m_pRender)
		return;

	m_pRender->SetWinWidth(w);
	m_pRender->SetWinHeight(h);
	m_pRender->SetDeviceWidth(w);
	m_pRender->SetDeviceHeight(h);
	m_pRender->ResetDevice();

	// 后期做再resetdevice函数里
	//ITexture* pDepthTexture = m_pShadowMap->GetDepthTexture();
	//pDepthTexture->Generate();
	//IFrameBuffer* pFrameBuffer = m_pShadowMap->GetFrameBuffer();
	//pFrameBuffer->AttachTexture(pDepthTexture, AT_DEPTH);

	//m_pRenderContext->pDepthRenderBuffer->Generate();
	//m_pRenderContext->pFBO->AttackRenderBuffer(m_pCurRenderText->pDepthRenderBuffer, AT_DEPTH);

	m_pRenderContext->pRenderTexture->Generate();
	m_pRenderContext->pFBO->AttachTexture(m_pRenderContext->pRenderTexture, AT_COLOR);
}

YY_OBJECTID GUI::GetDesktopForm() const
{
	if (!m_pDeskTop)
		return YY_INVALID_OBJECTID;

	return m_pDeskTop->GetID();
}

void GUI::AddModalForm(YY_OBJECTID control)
{
	m_modalForms.push_back(control);
}

void GUI::RemoveModalForm(YY_OBJECTID control)
{
	std::vector<YY_OBJECTID>::iterator itor = std::find(m_modalForms.begin(), m_modalForms.end(), control);
	if (itor == m_modalForms.end())
		return;

	m_modalForms.erase(itor);
}



// desktop form, modal from
// child1, child2, c3,c4 from form1
// child1, c2, c3 from child1
//scene tree.
void GUI::onEvent_AddBatch(const char* name, const YY::VarList& args)
{
	if (!m_pDeskTop)
		return;

	visitEntities(m_pDeskTop);

	// add all render object to painter batch group.
	int nModalFormsNum = m_modalForms.size();
	for (int i = 0; i < nModalFormsNum; i++)
	{
		YY_OBJECTID formID = m_modalForms[i];
		BaseObject* pObject = GetMgr()->Find(formID);
		if(nullptr == pObject)
			continue;

		if(!pObject->IsInstanceOf("Entity"))
			continue;

		Entity* pForm = (Entity*)pObject;
		visitEntities(pForm);
	}
}


void GUI::visitEntities(Entity* pEntity)
{
	// components
	std::vector<Component*> components = pEntity->GetAllComponents();
	int nCnt = components.size();
	for (int i = 0; i < nCnt; i++)
	{
		Component* pComponent = components[i];
		if(!pComponent->IsInstanceOf("RenderComponent"))
			continue;

		RenderComponent* pRenderComponent = (RenderComponent*)pComponent;
		pRenderComponent->OnAddBatch(m_pUIBatchGroup);
	}
		 
	// children
	int nChildNum = pEntity->GetChildCount();
	for (int i = 0; i < nChildNum; i++)
	{
		YY_OBJECTID id = pEntity->GetChildByIndex(i);
		BaseObject* pObject = GetMgr()->Find(id);
		if (nullptr == pObject)
		 	continue;
		 
		if (!pObject->IsInstanceOf("Entity"))
		 	continue;
		 
		Entity* pChildEntity = (Entity*)pObject;
		visitEntities(pChildEntity);
	}
}