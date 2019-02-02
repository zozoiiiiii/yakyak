#include "anchor_fill_component.h"



void AnchorFillComponent::OnCreate(const VariantMap& args)
{
	m_bAnchorFillParent = false;
}



void AnchorFillComponent::OnAdded()
{
}

void AnchorFillComponent::OnRenderBefore(RenderContext* pCxt)
{
	Item* pOwner = GetItem();
	if(pOwner->IsTransformChanged() || GetOwner()->FirstRender())
		UpdateFollowerItems();
}


void AnchorFillComponent::SetAnchorFillParent(bool bFlag)
{
	if (m_bAnchorFillParent == bFlag)
		return;

	m_bAnchorFillParent = bFlag;
	Entity* parent = GetOwner()->FindParent();
	if (nullptr == parent)
		return;

	AnchorFillComponent* pComponent = (AnchorFillComponent*)parent->FindComponent("AnchorFillComponent");
	if (nullptr == pComponent)
		return;

	if (m_bAnchorFillParent)
	{
		pComponent->m_followChildren.push_back(GetOwner()->GetID());
	}
	else
	{
		auto itor = std::find(pComponent->m_followChildren.begin(), pComponent->m_followChildren.end(), GetOwner()->GetID());
		if (itor != pComponent->m_followChildren.end())
			pComponent->m_followChildren.erase(itor);
	}
}

void AnchorFillComponent::OnOwnerEvent(const std::string& event, const YY::VarList& args)
{
	if (event == "AddChild")
	{
		YY_OBJECTID id = args.GetInt64(0);
		BaseObject* pObject = GetMgr()->Find(id);
		if (nullptr == pObject)
			return;

		if (!pObject->IsInstanceOf("Item"))
			return;

		Item* pItem = (Item*)pObject;
		AnchorFillComponent* pComponent = (AnchorFillComponent*)pItem->FindComponent("AnchorFillComponent");
		if (nullptr == pComponent)
			return;

		if (!pComponent->m_bAnchorFillParent)
			return;


		m_followChildren.push_back(id);
	}
	else if (event == "RemoveChild")
	{
		YY_OBJECTID id = args.GetInt64(0);
		auto itor = std::find(m_followChildren.begin(), m_followChildren.end(), id);
		if (itor != m_followChildren.end())
			m_followChildren.erase(itor);
	}
	else if (event == "ClearChildren")
	{
		m_followChildren.clear();
	}
}


void AnchorFillComponent::UpdateFollowerItems()
{
	int nCnt = m_followChildren.size();
	if (nCnt <= 0)
		return;


	Item* pOwner = GetItem();
	
	float fAbsLeft = pOwner->GetAbsLeft();
	float fAbsTop = pOwner->GetAbsTop();
	float fWidth = pOwner->GetWidth();
	float fHeight = pOwner->GetHeight();
	
	for (int i = 0; i < nCnt; i++)
	{
		YY_OBJECTID id = m_followChildren[i];
		BaseObject* pObject = GetMgr()->Find(id);
		if (nullptr == pObject)
			return;

		if (!pObject->IsInstanceOf("Item"))
			return;
		Item* pChildItem = (Item*)pObject;
		pChildItem->SetAbsLeft(fAbsLeft);
		pChildItem->SetAbsTop(fAbsTop);
		pChildItem->SetWidth(fWidth);
		pChildItem->SetHeight(fHeight);
	}
}