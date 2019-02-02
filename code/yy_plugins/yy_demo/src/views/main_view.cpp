#include "main_view.h"



MainView::MainView(IObjectMgr* pMgr)
{

	Item* pItem = (Item*)Entity::ParseFromEntityFile(pMgr, "views\\main.xml");
	return;
}

MainView::~MainView()
{

}

void MainView::Show()
{

}

void MainView::Close()
{

}