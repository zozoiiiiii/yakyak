/************************************************************************/
/*
@author:  junliang
@brief:
@time:    2018/12/29
*/
/************************************************************************/
#pragma once
#include "core/inc/yy_entity.h"


NS_YY_BEGIN
class RowComponent : public Component
{
	YY_BEGIN(RowComponent, Component);
	YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroyed();
	virtual void OnEvent(const std::string& event, const YY::VarList& args = YY::VarList());

private:
	void OnChildAdded(int index);
	void OnChildRemoved(int index);
};

NS_YY_END