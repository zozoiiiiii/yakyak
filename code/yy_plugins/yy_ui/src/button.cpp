#include "button.h"
#include "yy_ui/inc/i_gui.h"
#include "rectangle.h"
#include "mouse_area.h"
#include "anchor_fill_component.h"

NS_YY_BEGIN


void Button::OnCreate(const VariantMap& args)
{
	Rectangle* pRectangle = (Rectangle*)GetMgr()->Create("Rectangle");
	AddChild(pRectangle);

	// text

	{
		Entity* pMouseArea = (Entity*)GetMgr()->Create("MouseArea");
		AddChild(pMouseArea);
		GetEventMgr()->Connect(pMouseArea->GetID(), "Pressed", fastdelegate::MakeDelegate(this, &Button::onEvent_Pressed));
		GetEventMgr()->Connect(pMouseArea->GetID(), "Released", fastdelegate::MakeDelegate(this, &Button::onEvent_Released));
	}
}

void Button::OnDestroy()
{
	Component* pMouseArea = FindComponent("MouseArea");
	if(pMouseArea)
		GetMgr()->Destroy(pMouseArea->GetID());
}


void Button::onEvent_Pressed(const char* name, const YY::VarList& args)
{

}

void Button::onEvent_Released(const char* name, const YY::VarList& args)
{

}
NS_YY_END