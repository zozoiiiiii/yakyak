/************************************************************************/
/*
@author:  junliang
@brief:   scroll bar
@time:    2018/10/25
*/
/************************************************************************/
#pragma once

#include "../item.h"

NS_YY_BEGIN
class Rectangle;
class ScrollBar: public Item
{
	YY_BEGIN(ScrollBar, Item);
	YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	void SetColor(unsigned int color);
	unsigned int  GetColor();
private:
	unsigned int m_color;
	Rectangle* m_pRectangle;
};

NS_YY_END