#include "scroll_bar.h"
#include "yy_render/inc/i_render.h"
#include "../rectangle.h"
#include "../anchor_fill_component.h"


NS_YY_BEGIN
void ScrollBar::OnCreate(const VariantMap& args)
{
	Item::OnCreate(args);

	m_pRectangle = (Rectangle*)GetMgr()->Create("Rectangle");
	if (nullptr == m_pRectangle)
		return;
	AddChild(m_pRectangle);
}

void ScrollBar::OnDestroy()
{
}


void ScrollBar::SetColor(unsigned int color)
{
	m_color = color;
	m_pRectangle->SetColor(color);
}

unsigned int  ScrollBar::GetColor()
{
	return m_color;
}
NS_YY_END