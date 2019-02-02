/************************************************************************/
/*
@author:  junliang
@brief:   design panel
@time:    2018/12/04
*/
/************************************************************************/
#pragma once

#include "yy_ui/src/item.h"
#include "yy_ui/src/views/tree_view.h"

class Design : public BaseObject
{
	YY_BEGIN(Design, BaseObject);
	YY_END
public:
	static Design* Instance()
	{
		BaseObject* pObject = GetGlobalEntMgr()->FindGlobalObject("Design");
		if (NULL == pObject)
		{
			BaseObject* pBaseObject = GetGlobalEntMgr()->Create("Design");
			GetGlobalEntMgr()->SetGlobal("Design", pBaseObject->GetID());
			pObject = pBaseObject;
		}

		return (Design*)pObject;
	}

	virtual void OnCreate(const VariantMap& args);
	Item* GetPanel() { return m_pPanel; }
	void SetView(Item* pView);
	Item* GetView();

private:
	void LoadPanel();
private:
	Item* m_pPanel;
	Item* m_pContent;
	YY_OBJECTID m_view;
};