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

NS_YY_BEGIN
class ItemGeneratorComponent;
class ScrollViewDelegate;
class ScrollViewModel;
class ScrollView: public Item
{
    YY_BEGIN(ScrollView, Item);
    YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	virtual void OnRender(RenderContext* pCxt);
	virtual void Build();
	void SetDelegateItem(Item* pItem);
	void SetModel(BaseObject* pModel);

protected:
	BaseObject* GetModel();
	Item* GenerateDelegateItem();
	void SetRebuild();
private:
	void onEvent_DataChanged(const char* name, const YY::VarList& args);
private:
	ItemGeneratorComponent* m_pDelegate;
	BaseObject* m_pModel;
	bool m_bRebuild;
};

NS_YY_END