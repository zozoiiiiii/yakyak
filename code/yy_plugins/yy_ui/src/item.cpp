#include "item.h"
//#include "anchor_fill_component.h"
#include "yy_ui/inc/i_gui.h"

//NS_YY_BEGIN
Item::Item()
	:m_top(0.0f),m_left(0.0f),m_width(0.0f),m_height(0.0f),m_opacity(1),m_visible(false),m_bTransformChanged(false)
{}


void Item::OnCreate(const VariantMap& args)
{
	Entity::OnCreate(args);

	//AddComponent("AnchorFillComponent");
	//m_pAnchorLayout = AddComponent("AnchorLayoutComponent");
}

void Item::OnDestroyed()
{
	Entity::OnDestroy();
}
 
 void Item::Show()
 {
 	YY_OBJECTID id = IGUI::Instance(GetMgr())->GetDesktopForm();
 	Entity* pDeskTop = (Entity*)GetMgr()->Find(id);
 	if (NULL == pDeskTop)
 	{
 		return;
 	}
 
 	pDeskTop->AddChild(this);
 }

 void Item::Close()
 {
	 YY_OBJECTID id = IGUI::Instance(GetMgr())->GetDesktopForm();
	 Entity* pDeskTop = (Entity*)GetMgr()->Find(id);
	 if (NULL == pDeskTop)
	 {
		 return;
	 }

	 pDeskTop->RemoveChild(this);
 }

 void Item::ShowModal()
 {
	 IGUI::Instance(GetMgr())->AddModalForm(GetID());
 }
 
 void Item::CloseModal()
 {
 	IGUI::Instance(GetMgr())->RemoveModalForm(GetID());
 }

float Item::GetTop()
{
	return m_top; 
}

Item* Item::FindParentTransform()
{
	Entity* pParent = FindParent();
	if (nullptr == pParent)
		return nullptr;

	Component* pComponent = pParent->FindComponent("Item");
	if (!pComponent)
		return nullptr;

	return (Item*)pComponent;
}

float Item::GetAbsTop()
{
	Item* pParent = FindParentTransform();
	if (nullptr == pParent)
		return GetTop();

	float nParentTop = pParent->GetAbsTop();
	return nParentTop + GetTop();
}

float Item::GetLeft()
{
	return m_left; 
}

float Item::GetAbsLeft()
{
	Item* pParent = FindParentTransform();
	if (nullptr == pParent)
		return GetLeft();

	float nParentLeft = pParent->GetAbsLeft();
	return nParentLeft + GetLeft();
}

void Item::SetTop(float top)
{
	m_top = top;
	SetTransformChanged(); 
}


void Item::SetAbsTop(float top)
{
	Item* pParent = FindParentTransform();
	if (nullptr == pParent)
	{
		SetTop(top);
		return;
	}

	float nParentTop = pParent->GetAbsTop();
	SetTop(top - nParentTop);
}

void Item::SetLeft(float left)
{
	m_left = left;
	SetTransformChanged();
}

void Item::SetAbsLeft(float left)
{
	Item* pParent = FindParentTransform();
	if (nullptr == pParent)
	{
		SetLeft(left);
		return;
	}

	float nParentLeft = pParent->GetAbsLeft();
	SetLeft(left-nParentLeft);
}

float Item::GetWidth()
{
	return m_width; 
}

float Item::GetHeight()
{
	return m_height; 
}

void Item::SetWidth(float w) 
{
	m_width = w;
	SetTransformChanged();
}

void Item::SetHeight(float h)
{
	m_height = h;
	SetTransformChanged();
}

int Item::GetOpacity()
{
	return m_opacity; 
}

void Item::SetOpacity(int opacity)
{
	m_opacity = opacity;
}

bool Item::GetVisible()
{
	return m_visible; 
}

void Item::SetVisible(bool visible)
{
	m_visible = visible;
	if (m_visible)
		Show();
	else
		Close();
}


// void Item::OnAddBatch(IBatchGroup* pBatchGroup)
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


// void Item::OnRenderAfter(RenderContext* pCxt)
// {
// 	Entity::OnRenderAfter(pCxt);
// 	m_bTransformChanged = false;
// }

void Item::SetTransformChanged(bool bChanged)
{
	m_bTransformChanged = bChanged;
}


Item* ItemComponent::FindItem()
{
	if (!GetOwner()->IsInstanceOf("Item"))
		return nullptr;
	return (Item*)GetOwner();
}

//NS_YY_END