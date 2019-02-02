/************************************************************************/
/*
@author:  junliang
@brief:   scroll view
@time:    2018/10/25
*/
/************************************************************************/
#pragma once

#include "../item.h"
#include "scroll_bar.h"
#include <vector>

NS_YY_BEGIN


class ScrollViewModel : public BaseObject
{
	YY_BEGIN(ScrollViewModel, BaseObject);
	YY_END
public:
	YY::Var GetData(int row, int column)
	{
		YY_OBJECTID id = m_objects[row];
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

	void Insert(const YY::VarList& args);
	void Remove(const YY::VarList& args);
	void Update(const YY::VarList& args);
	int GetCount() { return m_objects.size(); }

private:
	std::vector<YY_OBJECTID> m_objects;
};
NS_YY_END