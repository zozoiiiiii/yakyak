#include "scroll_view_delegate.h"
#include "../rectangle.h"
#include "../mouse_area.h"
#include "../anchor_fill_component.h"
#include "yy_core.h"

void ScrollViewDelegate::OnCreate(const VariantMap& args)
{
	m_index = 0;
	Item::OnCreate(args);
}

void ScrollViewDelegate::Init()
{

	//GetEventMgr()->Connect(pMouseArea->GetID(), "Pressed", fastdelegate::MakeDelegate(this, &ScrollViewDelegate::onEvent_Pressed));
	//GetEventMgr()->Connect(pMouseArea->GetID(), "Released", fastdelegate::MakeDelegate(this, &ScrollViewDelegate::onEvent_Released));
}

void ScrollViewDelegate::SetIndex(int index)
{
	m_index = index;
	Rectangle* pRect = (Rectangle*)FindChild("rect");
	if (pRect)
	{
		AnchorFillComponent* pComponent = (AnchorFillComponent*)pRect->FindComponent("AnchorFillComponent");
		pComponent->SetAnchorFillParent(true);
	}

	if (m_index % 2 == 0)
	{
		pRect->SetColor(COLOR_TREEITEM_HIGHLIGHT);
	}

	std::string str = YY::Int32ToStr(m_index);
	Text* pText = (Text*)FindChild("text");
	if(pText)
		pText->SetText(str);
	//m_pRectangle->SetWidth(GetWidth()-2);
	//m_pRectangle->SetHeight(GetHeight() - 25);

	//m_pRectangle->SetLeft(0);
	//m_pRectangle->SetTop(m_index*GetHeight());

	
}


void ScrollViewDelegate::onEvent_Pressed(const char* name, const YY::VarList& args)
{
	int x = args.GetInt(0);
	int y = args.GetInt(1);
	LOG_TRACE("pressed, x:"<<x <<", y:"<< y<<", index:"<<m_index);
}

void ScrollViewDelegate::onEvent_Released(const char* name, const YY::VarList& args)
{
}