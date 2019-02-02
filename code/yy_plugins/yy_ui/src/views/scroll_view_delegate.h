/************************************************************************/
/*
@author:  junliang
@brief:   scroll view
@time:    2018/10/25
*/
/************************************************************************/
#pragma once

#include "../item.h"
#include "scroll_bar.h"
#include <vector>
#include "../rectangle.h"
#include "../text.h"

NS_YY_BEGIN

class ScrollView;
class ScrollViewModel;
class MouseArea;
class ScrollViewDelegate : public Item
{
	YY_BEGIN(ScrollViewDelegate, Item);
	YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	void Init();
	void SetIndex(int index);
	//void SetScrollView(ScrollView* pView) { m_pView = pView; }

private:
	void onEvent_Pressed(const char* name, const YY::VarList& args);
	void onEvent_Released(const char* name, const YY::VarList& args);
private:
	int m_index;
	//Rectangle* m_pRectangle;
	//Text* m_pText;
	//ScrollView* m_pView;
	//MouseArea* m_pMouseArea;
};


NS_YY_END