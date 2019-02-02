#include "design_view.h"
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
#include "hierarchy_view.h"

void Design::OnCreate(const VariantMap& args)
{
	LoadPanel();
}


void Design::SetView(Item* pView)
{
	m_pContent->AddChild(pView);
	m_view = pView->GetID();
}


Item* Design::GetView()
{
	return (Item*)GetMgr()->Find(m_view);
}

void Design::LoadPanel()
{
	m_pPanel = (Item*)GetMgr()->Create("Item");
	m_pPanel->SetName("HierarchyPanel");

	Item* pHierarchyPanel;// = Hierarchy::Instance()->GetPanel();
	m_pPanel->SetAbsLeft(pHierarchyPanel->GetAbsLeft() + pHierarchyPanel->GetWidth());
	m_pPanel->SetTop(0);
	m_pPanel->SetWidth(400);
	m_pPanel->SetHeight(pHierarchyPanel->GetHeight());

	Item* pContent = (Item*)GetMgr()->Create("Item");
	m_pContent = pContent;
	pContent->SetName("Content");
	pContent->SetTop(20);
	pContent->SetLeft(20);
	pContent->SetWidth(m_pPanel->GetWidth() - 40);
	pContent->SetHeight(m_pPanel->GetHeight() - 40);
	m_pPanel->AddChild(pContent);

	m_pPanel->Show();
}