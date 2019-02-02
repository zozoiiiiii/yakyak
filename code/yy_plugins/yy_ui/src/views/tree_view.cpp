#include "tree_view.h"
#include "scroll_view_delegate.h"
#include "scroll_view_model.h"
#include "tree_view_model.h"

NS_YY_BEGIN


void TreeView::OnCreate(const VariantMap& args)
{
	ScrollView::OnCreate(args);
}

void TreeView::OnDestroy()
{
	ScrollView::OnDestroy();
}

void TreeView::Build()
{
	ClearChildren();

	Item* pContent = (Item*)GetMgr()->Create("Item");
	pContent->SetName("Content");
	AddChild(pContent);

	TreeViewModel* pModel = (TreeViewModel*)GetModel();
	YY_OBJECTID id = pModel->GetRoot();
	Entity* pRoot = (Entity*)GetMgr()->Find(id);
	if (nullptr == pRoot)
		return;

	ScrollViewDelegate* pItem = (ScrollViewDelegate*)GenerateDelegateItem();
	pItem->SetName("ScrollViewDelegateItem");
	pItem->SetWidth(GetWidth());
	pItem->SetHeight(50);
	pItem->SetLeft(0);
	pItem->SetTop(0);
	pItem->SetIndex(0);
	BuildFromEntity(pRoot, pItem);
	pContent->AddChild(pItem);
}

void TreeView::BuildFromEntity(Entity* pEntity, ScrollViewDelegate* pItem)
{
	int nCnt = pEntity->GetChildCount();
	for (int i = 0; i < nCnt; i++)
	{
		YY_OBJECTID id = pEntity->GetChildByIndex(i);
		Entity* pChildEntity = (Entity*)GetMgr()->Find(id);
		if(NULL == pChildEntity)
			continue;;

		Item* pChildItem = GenerateDelegateItem();
		if (!pChildItem->IsInstanceOf("ScrollViewDelegate"))
		{
			GetMgr()->Destroy(pChildItem->GetID());
			continue;
		}

		ScrollViewDelegate* pDelegateItem = (ScrollViewDelegate*)pChildItem;
		pDelegateItem->SetName("ScrollViewDelegateItem");
		pDelegateItem->SetWidth(GetWidth());
		pDelegateItem->SetHeight(50);
		pDelegateItem->SetLeft(30);
		pDelegateItem->SetTop(pItem->GetHeight() + i * 50);
		pDelegateItem->SetIndex(i);
		pItem->AddChild(pDelegateItem);

		BuildFromEntity(pChildEntity, pDelegateItem);
	}
}
NS_YY_END