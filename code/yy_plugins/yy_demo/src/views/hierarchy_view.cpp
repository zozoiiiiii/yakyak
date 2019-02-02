#include "hierarchy_view.h"
#include "yy_core.h"
#include "yy_ui/inc/i_gui.h"
#include "yy_ui/src/rectangle.h"
#include "yy_ui/src/views/scroll_view.h"
#include "yy_ui/src/views/scroll_view_delegate.h"
#include "yy_ui/src/views/scroll_view_model.h"
#include "yy_ui/src/item_generator_component.h"
#include "yy_ui/src/image.h"
#include "yy_ui/src/text.h"
#include "yy_ui/src/anchor_fill_component.h"
#include "yy_ui/src/views/tree_view_model.h"
#include "design_view.h"

HierarchyView::HierarchyView(IObjectMgr* pMgr)
{
	m_pPanel = (Item*)YY::Entity::ParseFromEntityFile(pMgr, "views\\hierarchy.xml");
	return;

	/*
	//YY_OBJECTID deskTopID = IGUI::Instance()->GetDesktopForm();
	m_pPanel = (Item*)pMgr->Create("Item");
	m_pPanel->SetName("HierarchyPanel");
	{
		Item* pForm = m_pPanel;
		pForm->SetLeft(0);
		pForm->SetTop(0);
		pForm->SetWidth(100);
		pForm->SetHeight(500);

		TreeView* pTreeView = (TreeView*)pMgr->Create("TreeView");
		pTreeView->SetName("ScrollView");
		pTreeView->SetLeft(0);
		pTreeView->SetTop(0);
		pTreeView->SetWidth(pForm->GetWidth());
		pTreeView->SetHeight(pForm->GetHeight());

		// used for get list data
		TreeViewModel* pModel = (TreeViewModel*)GetMgr()->Create("TreeViewModel");
		m_pTreeModel = pModel;
		pTreeView->SetModel(pModel);

		// used for create list item
		pTreeView->SetDelegateItem(LoadDelegateItem());



		// test data
		if(0)
		{

			YY::Item* pItem = (Item*)GetMgr()->Create("Item");
			YY::Rectangle* pRectangle = (YY::Rectangle*)GetMgr()->Create("Rectangle");
			pItem->AddChild(pRectangle);

			YY::Rectangle* pRectangle1 = (YY::Rectangle*)GetMgr()->Create("Rectangle");
			pItem->AddChild(pRectangle1);

			pModel->SetRoot(pItem->GetID());
		}

		pForm->AddChild(pTreeView);
		pForm->Show();
	}

	return;
	*/
}


Item* HierarchyView::LoadDelegateItem()
{
	YY::BaseObject* pObject = GetMgr()->Create("ScrollViewDelegate");
	if (nullptr == pObject)
		return nullptr;

	ScrollViewDelegate* pScrollViewDelegate = (ScrollViewDelegate*)pObject;

	YY::Rectangle* pRectangle = (YY::Rectangle*)GetMgr()->Create("Rectangle");
	pRectangle->SetName("rect");
	pRectangle->SetColor(COLOR_EDITOR);
	AnchorFillComponent* pComponent = (AnchorFillComponent*)pRectangle->FindComponent("AnchorFillComponent");
	pComponent->SetAnchorFillParent(true);
	pScrollViewDelegate->AddChild(pRectangle);



	// text use free type library.
	Text* pText = (Text*)GetMgr()->Create("Text");
	pText->SetName("text");
	pText->SetText("Hello,World!");
	pText->SetTop(20);
	pText->SetLeft(20);
	pText->SetWidth(100);
	pText->SetHeight(100);
	pScrollViewDelegate->AddChild(pText);

	Entity* pMouseArea = (Entity*)GetMgr()->Create("MouseArea");
	pScrollViewDelegate->AddChild(pMouseArea);
	return pScrollViewDelegate;
}


void HierarchyView::SetRoot(Item* pRoot)
{
	// set data model
	m_pTreeModel->SetRoot(pRoot->GetID());
}