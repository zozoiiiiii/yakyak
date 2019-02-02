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
class ColumnComponent : public Component
{
	YY_BEGIN(ColumnComponent, Component);
	YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroyed();

private:
};

NS_YY_END