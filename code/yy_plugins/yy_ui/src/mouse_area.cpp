#include "mouse_area.h"
#include "yy_ui/inc/i_gui.h"


NS_YY_BEGIN


void MouseArea::OnCreate(const VariantMap& args)
{
	Item::OnCreate(args);

	GetMgr()->AddMsgProc(this, WM_LBUTTONDOWN);
	GetMgr()->AddMsgProc(this, WM_LBUTTONUP);
}

void MouseArea::OnDestroy()
{
	GetMgr()->RemoveMsgProc(this, WM_LBUTTONDOWN);
	GetMgr()->RemoveMsgProc(this, WM_LBUTTONUP);
}

bool MouseArea::OnMsg(uint32 msg_id, uint32 param1, uint32 param2)
{
	switch (msg_id)
	{
	case WM_LBUTTONDOWN:
		OnPressed(msg_id, param1, param2);
		break;
	case WM_LBUTTONUP:
		OnReleased(msg_id, param1, param2);
		break;
	default:
		break;
	}
	return true; 
}

bool MouseArea::isInside(int x, int y)
{
	int x1 = GetAbsLeft();
	int y1 = GetAbsTop();
	int x2 = x1 + GetWidth();
	int y2 = y1 + GetHeight();
	if (x >= x1&&y >= y1 
		&&x <= x2&& y <= y2)
	{
		return true;
	}

	return false;
}

void MouseArea::OnPressed(uint32 msg_id, uint32 param1, uint32 param2)
{
	int xPos = LOWORD(param2);
	int yPos = HIWORD(param2);
	if(isInside(xPos, yPos))
		GetEventMgr()->Invoke(GetID(), "Pressed", YY::VarList()<<xPos<<yPos);
}

void MouseArea::OnReleased(uint32 msg_id, uint32 param1, uint32 param2)
{
	int xPos = LOWORD(param2);
	int yPos = HIWORD(param2);
	if (isInside(xPos, yPos))
		GetEventMgr()->Invoke(GetID(), "Released", YY::VarList() << xPos<<yPos);
}

NS_YY_END