/************************************************************************/
/*
@author:  junliang
@brief:   consider qml rectangle
@time:    2018/09/28
*/
/************************************************************************/
#pragma once
#include "yy_core.h"

//NS_YY_BEGIN
class RectangleComponent: public YY::Component
{
	YY_BEGIN(RectangleComponent, Component);
	YY_FIELD(&RectangleComponent::m_color, "color", "");
    YY_END

public:
	RectangleComponent();
	void SetColor(unsigned int color);
	unsigned int  GetColor();
private:
	sint32 m_color;	// unsigned int and signed int has same data, different rule. here use sint32 instead of unsigned int, as reflection module not support unsigned int.
};

//NS_YY_END