/************************************************************************/
/*
@author:  junliang
@brief:   tree view
@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "scroll_view.h"
NS_YY_BEGIN
class TreeView: public ScrollView
{
    YY_BEGIN(TreeView, ScrollView);
    YY_END

private:
	enum { TREE_LINE_COLOR = 0xFFC0C0C0 };
	enum { MIN_ITEM_HEIGHT = 4 };
	enum { MIN_LEVEL_WIDTH = 10 };
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	virtual void Build();

private:
	void BuildFromEntity(Entity* pEntity, ScrollViewDelegate* pItem);
};

NS_YY_END