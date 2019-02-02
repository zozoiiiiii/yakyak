/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/10/25
*/
/************************************************************************/
#pragma once

#include "item.h"
#include <vector>
#include "item_component.h"
NS_YY_BEGIN

class AnchorLayoutComponent : public ItemComponent
{
	YY_BEGIN(AnchorLayoutComponent, ItemComponent);
	YY_FIELD(&AnchorLayoutComponent::m_fill_item, "fill", "");
	YY_FIELD(&AnchorLayoutComponent::m_left_item, "left", "");
	YY_FIELD(&AnchorLayoutComponent::m_left_margin, "leftMargin", "");
	YY_FIELD(&AnchorLayoutComponent::m_right_item, "right", "");
	YY_FIELD(&AnchorLayoutComponent::m_right_margin, "rightMargin", ""); 
	YY_END
public:
	//virtual void OnCreate(const VariantMap& args);
	
private:
	std::string m_fill_item;
	std::string m_left_item;
	int m_left_margin;
	std::string m_right_item;
	int m_right_margin;
};


NS_YY_END