#include "demo.h"
#include "yy_core.h"
#include "yy_ui/inc/i_gui.h"
//#include "yy_ui/src/views/scroll_view.h"
//#include "yy_ui/src/views/scroll_view_delegate.h"
//#include "yy_ui/src/views/scroll_view_model.h"
//#include "yy_ui/src/item_generator_component.h"
//#include "yy_ui/src/image.h"
//#include "yy_ui/src/text.h"
//#include "views/hierarchy_view.h"
//#include "views/design_view.h"
//#include "views/main_view.h"
//#include "yy_ui/src/rectangle.h"
//#include "yy_ui/src/item_transform_component.h"
#include "yy_ui/src/rectangle_component.h"
//#include "yy_ui/src/rectangle_render_component.h"
#include "yy_ui/src/text_component.h"
#include "yy_ui/src/image_component.h"
//#include "yy_ui/src/positioner/row_component.h"
//#include "yy_ui/src/positioner/column_component.h"


using namespace YY;
void Demo::OnCreate(const VariantMap& args)
{
	YY::Entity* pMainEntity = (YY::Entity*)GetMgr()->Create("Entity");

	{
		Item* pItem = (Item*)GetMgr()->Create("Item");
		RectangleComponent* pRectangle = (RectangleComponent*)pItem->AddComponent("RectangleComponent");
		pRectangle->SetColor(COLOR_GREE);
		pItem->SetWidth(100);
		pItem->SetHeight(100);
		pItem->SetVisible(true);
		pMainEntity->AddChild(pItem);
	}

	{
		Item* pItem = (Item*)GetMgr()->Create("Item");
		TextComponent* pText = (TextComponent*)pItem->AddComponent("TextComponent");
		pText->SetText("hello, world");
		pItem->SetLeft(100);
		pItem->SetWidth(100);
		pItem->SetHeight(100);
		pItem->SetVisible(true);
		pMainEntity->AddChild(pItem);
	}

	{
		Item* pItem = (Item*)GetMgr()->Create("Item");
		ImageComponent* pImage = (ImageComponent*)pItem->AddComponent("ImageComponent");
		pImage->SetImageSource("ui_image/grass.bmp");
		pItem->SetLeft(500);
		//pTransform->SetWidth(100);
		//pTransform->SetHeight(100);
		pItem->SetVisible(true);
		pMainEntity->AddChild(pItem);
	}

	pMainEntity->SerializeToFile("ttt.xml");



	// create gui and render module
	//IGUI::Instance();
	//MainView* pMainView = new MainView(GetMgr());
	return;
// 	float width = IRender::Instance()->GetDeviceWidth();
// 	float height = IRender::Instance()->GetDeviceHeight();
// 
// 	// load view file
// 	Item* pView = (Item*)Entity::ParseFromEntityFile(GetMgr(), "views\\main.xml");
// 	if (!pView)
// 		return;
// 
// 	//HierarchyView::Instance()->SetRoot(pView);
// 	Design::Instance()->SetView(pView);
// 
// 	return;
// 	Item* pForm_Property = (Item*)GetMgr()->Create("Item");
// 	{
// 		Item* pForm = pForm_Property;
// 		pForm->SetWidth(100);
// 		pForm->SetHeight(height);
// 		YY::Rectangle* pRectangle = (YY::Rectangle*)GetMgr()->Create("Rectangle");
// 		pRectangle->SetAbsLeft(width-pForm->GetWidth());
// 		pRectangle->SetAbsTop(0);
// 		pRectangle->SetWidth(400);
// 	//	pRectangle->SetHeight(height- pHierarchyPanel->GetHeight());
// 		pRectangle->SetColor(COLOR_GREE);
// 		pForm->AddChild(pRectangle);
// 		pForm->ShowModal();
// 	}
}

void Demo::OnDestroy()
{
    //GetMgr()->RemoveExcute(this);
}

void Demo::OnExcute(float sec)
{
}

