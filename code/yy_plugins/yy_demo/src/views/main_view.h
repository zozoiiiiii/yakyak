/************************************************************************/
/*
@author:  junliang
@brief:   dock views
@time:    2018/12/04
*/
/************************************************************************/
#pragma once


#include "yy_ui/src/item.h"
#include "reflection/yy_reflection.h"

class MainView
{
public:
	MainView(IObjectMgr* pMgr);
	~MainView();

	void Show();
	void Close();

private:
};