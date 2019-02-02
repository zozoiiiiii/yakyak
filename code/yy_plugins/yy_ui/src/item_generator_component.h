/************************************************************************/
/*
@author:  junliang
@brief:   like qml component， used for create item instance.
@time:    2018/10/25
*/
/************************************************************************/
#pragma once

#include "item.h"
#include <vector>
#include "item_component.h"
NS_YY_BEGIN

class ItemGeneratorComponent : public ItemComponent
{
	YY_BEGIN(ItemGeneratorComponent, ItemComponent);
	YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	void SetTemplateItem(Item* pItem) { m_pItem = pItem; }

	Item* Create();
private:
	Item* m_pItem;
	int m_index;
};


NS_YY_END