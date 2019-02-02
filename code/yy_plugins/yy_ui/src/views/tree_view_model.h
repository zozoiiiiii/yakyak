/************************************************************************/
/*
@author:  junliang
@brief:   tree view model
@time:    2018/10/25
*/
/************************************************************************/
#pragma once

#include "../item.h"
#include "scroll_bar.h"
#include <vector>

NS_YY_BEGIN


class TreeViewModel : public Item
{
	YY_BEGIN(TreeViewModel, Item);
	YY_END
public:
	YY::Var GetData(int row, int column)
	{
		//YY_OBJECTID id = m_objects[row];
		BaseObject* pObject;
		switch (column)
		{
		case 0:
			return "";
			break;
		default:
			break;
		}

		return "";
	}

	void SetRoot(YY_OBJECTID id);
	YY_OBJECTID GetRoot() { return m_root; }
private:
	YY_OBJECTID m_root;
};
NS_YY_END