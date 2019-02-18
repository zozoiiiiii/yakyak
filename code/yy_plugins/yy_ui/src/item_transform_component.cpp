#include "item_transform_component.h"
//#include "anchor_fill_component.h"
#include "yy_ui/inc/i_gui.h"

//NS_YY_BEGIN
ItemTransformComponent::ItemTransformComponent()
	:m_top(0.0f),m_left(0.0f),m_width(0.0f),m_height(0.0f),m_opacity(1),m_visible(false),m_bTransformChanged(false)
{}


void ItemTransformComponent::OnCreate(const VariantMap& args)
{
	Component::OnCreate(args);

	//AddComponent("AnchorFillComponent");
	//m_pAnchorLayout = AddComponent("AnchorLayoutComponent");
}

void ItemTransformComponent::OnDestroyed()
{
	Component::OnDestroy();
}
 
 void ItemTransformComponent::Show()
 {
 	YY_OBJECTID id = IGUI::Instance()->GetDesktopForm();
 	Entity* pDeskTop = (Entity*)GetMgr()->Find(id);
 	if (NULL == pDeskTop)
 	{
 		return;
 	}
 
 	pDeskTop->AddChild(GetOwner());
 }

 void ItemTransformComponent::Close()
 {
	 YY_OBJECTID id = IGUI::Instance()->GetDesktopForm();
	 Entity* pDeskTop = (Entity*)GetMgr()->Find(id);
	 if (NULL == pDeskTop)
	 {
		 return;
	 }

	 pDeskTop->RemoveChild(GetOwner());
 }

 void ItemTransformComponent::ShowModal()
 {
	 IGUI::Instance()->AddModalForm(GetID());
 }
 
 void ItemTransformComponent::CloseModal()
 {
 	IGUI::Instance()->RemoveModalForm(GetID());
 }

float ItemTransformComponent::GetTop()
{
	return m_top; 
}

ItemTransformComponent* ItemTransformComponent::FindParentTransform()
{
	Entity* pParent = GetOwner()->FindParent();
	if (nullptr == pParent)
		return nullptr;

	Component* pComponent = pParent->FindComponent("ItemTransformComponent");
	if (!pComponent)
		return nullptr;

	return (ItemTransformComponent*)pComponent;
}

float ItemTransformComponent::GetAbsTop()
{
	ItemTransformComponent* pParent = FindParentTransform();
	if (nullptr == pParent)
		return GetTop();

	float nParentTop = pParent->GetAbsTop();
	return nParentTop + GetTop();
}

float ItemTransformComponent::GetLeft()
{
	return m_left; 
}

float ItemTransformComponent::GetAbsLeft()
{
	ItemTransformComponent* pParent = FindParentTransform();
	if (nullptr == pParent)
		return GetLeft();

	float nParentLeft = pParent->GetAbsLeft();
	return nParentLeft + GetLeft();
}

void ItemTransformComponent::SetTop(float top)
{
	m_top = top;
	SetTransformChanged(); 
}


void ItemTransformComponent::SetAbsTop(float top)
{
	ItemTransformComponent* pParent = FindParentTransform();
	if (nullptr == pParent)
	{
		SetTop(top);
		return;
	}

	float nParentTop = pParent->GetAbsTop();
	SetTop(top - nParentTop);
}

void ItemTransformComponent::SetLeft(float left)
{
	m_left = left;
	SetTransformChanged();
}

void ItemTransformComponent::SetAbsLeft(float left)
{
	ItemTransformComponent* pParent = FindParentTransform();
	if (nullptr == pParent)
	{
		SetLeft(left);
		return;
	}

	float nParentLeft = pParent->GetAbsLeft();
	SetLeft(left-nParentLeft);
}

float ItemTransformComponent::GetWidth()
{
	return m_width; 
}

float ItemTransformComponent::GetHeight()
{
	return m_height; 
}

void ItemTransformComponent::SetWidth(float w) 
{
	m_width = w;
	SetTransformChanged();
}

void ItemTransformComponent::SetHeight(float h)
{
	m_height = h;
	SetTransformChanged();
}

int ItemTransformComponent::GetOpacity()
{
	return m_opacity; 
}

void ItemTransformComponent::SetOpacity(int opacity)
{
	m_opacity = opacity;
}

bool ItemTransformComponent::GetVisible()
{
	return m_visible; 
}

void ItemTransformComponent::SetVisible(bool visible)
{
	m_visible = visible;
	if (m_visible)
		Show();
	else
		Close();
}


// void ItemTransformComponent::OnAddBatch(IBatchGroup* pBatchGroup)
// {
// 	pBatchGroup->AddGUIBatch(this);
// 
// 	int nChildNum = GetChildCount();
// 	for (int i = 0; i < nChildNum; i++)
// 	{
// 		YY_OBJECTID id = GetChildByIndex(i);
// 		BaseObject* pObject = GetMgr()->Find(id);
// 		if (nullptr == pObject)
// 			continue;
// 
// 		if (!pObject->IsInstanceOf("Item"))
// 			continue;
// 
// 		Item* pItem = (Item*)pObject;
// 		pItem->OnAddBatch(pBatchGroup);
// 	}
// }


// void ItemTransformComponent::OnRenderAfter(RenderContext* pCxt)
// {
// 	Entity::OnRenderAfter(pCxt);
// 	m_bTransformChanged = false;
// }

void ItemTransformComponent::SetTransformChanged(bool bChanged)
{
	m_bTransformChanged = bChanged;
}

//NS_YY_END