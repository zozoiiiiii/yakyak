#include "scroll_view.h"
#include "yy_ui/inc/i_gui.h"
#include "scroll_view_delegate.h"
#include "scroll_view_model.h"
#include "../item_generator_component.h"
#include "../anchor_fill_component.h"


NS_YY_BEGIN


void ScrollView::OnCreate(const VariantMap& args)
{
	Item::OnCreate(args);
	m_bRebuild = true;

	Rectangle* pContent = (Rectangle*)GetMgr()->Create("Rectangle");
	AnchorFillComponent* pComponent = (AnchorFillComponent*)pContent->FindComponent("AnchorFillComponent");
	pComponent->SetAnchorFillParent(true);
	pContent->SetName("Content");
	AddChild(pContent);
}

void ScrollView::OnDestroy()
{
	Item::OnDestroy();
}

void ScrollView::OnRender(RenderContext* pCxt)
{
	if (m_bRebuild)
		Build();

	m_bRebuild = false;
}

void ScrollView::SetModel(BaseObject* pModel)
{
	m_pModel = pModel;
	GetEventMgr()->Disconnect(pModel->GetID(), "DataChanged", fastdelegate::MakeDelegate(this, &ScrollView::onEvent_DataChanged));
	GetEventMgr()->Connect(pModel->GetID(), "DataChanged", fastdelegate::MakeDelegate(this, &ScrollView::onEvent_DataChanged));
}


BaseObject* ScrollView::GetModel()
{
	return m_pModel;
}


void ScrollView::SetDelegateItem(Item* pItem)
{
	m_pDelegate = (ItemGeneratorComponent*)GetMgr()->Create("ItemGeneratorComponent");
	if (nullptr == m_pDelegate)
		return;

	//pItem->SerializeToFile("prefab\\scroll_delegate_item.xml");
	m_pDelegate->SetTemplateItem(pItem);
}


Item* ScrollView::GenerateDelegateItem()
{
	return m_pDelegate->Create();
}

void ScrollView::SetRebuild()
{
	m_bRebuild = true;
}

void ScrollView::onEvent_DataChanged(const char* name, const YY::VarList& args)
{
	m_bRebuild = true;
}


void ScrollView::Build()
{
	if (nullptr == m_pDelegate)
		return;

	Item* pContent = (Item*)FindChild("Content");
	if (nullptr == pContent)
		return;

	pContent->ClearChildren();


	// create new items
	ScrollViewModel* pModel = (ScrollViewModel*)GetModel();
	int nCnt = pModel->GetCount();
	for (int i = 0; i < nCnt; i++)
	{
		Item* pItem = GenerateDelegateItem();
		if (!pItem->IsInstanceOf("ScrollViewDelegate"))
		{
			GetMgr()->Destroy(pItem->GetID());
			continue;
		}

		ScrollViewDelegate* pDelegateItem = (ScrollViewDelegate*)pItem;
		pDelegateItem->SetName("ScrollViewDelegateItem");
		pDelegateItem->SetWidth(GetWidth());
		pDelegateItem->SetHeight(50);
		pDelegateItem->SetLeft(0);
		pDelegateItem->SetTop(i*50);
		pDelegateItem->SetIndex(i);

		pContent->AddChild(pItem);
	}
}

NS_YY_END