/************************************************************************/
/*
@author:  junliang
@brief:   hierarchy panel

panel
--treeview


@time:    2018/12/04
*/
/************************************************************************/
#pragma once
#include "yy_ui/src/views/tree_view.h"
#include "yy_ui/src/views/tree_view_model.h"
#include "reflection/yy_reflection.h"

class HierarchyView : public BaseObject
{
public:
	HierarchyView(IObjectMgr* pMgr);
	~HierarchyView();
	void Show();
	void Close();
	void SetRoot(Item* pRoot);
	Item* GetPanel() { return m_pPanel; }
private:
	void LoadPanel();
	Item* LoadDelegateItem();
private:
	Item* m_pPanel;
	YY::TreeViewModel* m_pTreeModel;
};