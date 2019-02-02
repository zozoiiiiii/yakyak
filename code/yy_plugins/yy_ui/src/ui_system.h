/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/09/28
*/
/************************************************************************/
#pragma once
#include "core/inc/yy_entity.h"

NS_YY_BEGIN
class UISystem : public System
{
    YY_BEGIN(UISystem, "System");
    YY_END
public:
	virtual void OnCreate();
	virtual void OnDestroy();
	void OnExcute(float sec);
	virtual void onComponentAdded(YY_OBJECTID id);
	virtual void onComponentRemoved(YY_OBJECTID id);

private:
	void onEvent_2DRender(const char* name, const YY::VarList& args);
private:

};
NS_YY_END