/************************************************************************/
/*
@author:  junliang
@brief:   consider qml rectangle
@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "item.h"

NS_YY_BEGIN
class MouseArea;
class Button: public Item
{
    YY_BEGIN(Button, Item);
    YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
private:
	void onEvent_Pressed(const char* name, const YY::VarList& args);
	void onEvent_Released(const char* name, const YY::VarList& args);

};

NS_YY_END