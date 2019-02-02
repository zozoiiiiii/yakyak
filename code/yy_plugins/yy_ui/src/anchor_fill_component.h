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

class AnchorFillComponent : public ItemComponent
{
	YY_BEGIN(AnchorFillComponent, ItemComponent);
	YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnAdded();
	virtual void OnRenderBefore(RenderContext* pCxt);
	virtual void SetAnchorFillParent(bool bFlag);
	virtual void OnOwnerEvent(const std::string& event, const YY::VarList& args);

private:
	void UpdateFollowerItems();
private:
	bool m_bAnchorFillParent;
	std::vector<YY_OBJECTID> m_followChildren;
};


NS_YY_END