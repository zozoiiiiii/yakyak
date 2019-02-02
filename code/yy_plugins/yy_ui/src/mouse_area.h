/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "item.h"

NS_YY_BEGIN
class MouseArea: public Item
{
    YY_BEGIN(MouseArea, Item);
    YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	virtual bool OnMsg(uint32 msg_id, uint32 param1, uint32 param2);
private:
	void OnPressed(uint32 msg_id, uint32 param1, uint32 param2);
	void OnReleased(uint32 msg_id, uint32 param1, uint32 param2);
	bool isInside(int x, int y);
};

NS_YY_END