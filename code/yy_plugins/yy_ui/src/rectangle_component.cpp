#include "rectangle_component.h"
#include "yy_ui/inc/i_gui.h"



NS_YY_BEGIN

RectangleComponent::RectangleComponent():m_color(0)
{}



void RectangleComponent::SetColor(unsigned int color)
{
	m_color = color;
}

unsigned int  RectangleComponent::GetColor()
{
	return m_color;
}


NS_YY_END