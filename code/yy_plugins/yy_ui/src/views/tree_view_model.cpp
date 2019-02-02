#include "tree_view_model.h"

NS_YY_BEGIN


void TreeViewModel::SetRoot(YY_OBJECTID id)
{
	m_root = id;
	GetEventMgr()->Invoke(GetID(), "DataChanged");
}
NS_YY_END