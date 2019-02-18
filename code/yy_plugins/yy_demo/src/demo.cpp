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
#include "yy_ui/src/item_transform_component.h"
#include "yy_ui/src/rectangle_component.h"
#include "yy_ui/src/rectangle_render_component.h"
#include "yy_ui/src/text_component.h"
#include "yy_ui/src/image_component.h"
//#include "yy_ui/src/positioner/row_component.h"
//#include "yy_ui/src/positioner/column_component.h"


using namespace YY;
void Demo::OnCreate(const VariantMap& args)
{
	YY::Entity* pMainEntity = (YY::Entity*)GetMgr()->Create("Entity");

	{
		YY::Entity* pEntity = (YY::Entity*)GetMgr()->Create("Entity");
		ItemTransformComponent* pTransform = (ItemTransformComponent*)pEntity->AddComponent("ItemTransformComponent");
		RectangleComponent* pRectangle = (RectangleComponent*)pEntity->AddComponent("RectangleComponent");
		RectangleRenderComponent* pRender = (RectangleRenderComponent*)pEntity->AddComponent("RectangleRenderComponent");
		pRectangle->SetColor(COLOR_GREE);
		pTransform->SetWidth(100);
		pTransform->SetHeight(100);
		pTransform->SetVisible(true);
		pMainEntity->AddChild(pEntity);
	}

	{
		YY::Entity* pEntity = (YY::Entity*)GetMgr()->Create("Entity");
		ItemTransformComponent* pTransform = (ItemTransformComponent*)pEntity->AddComponent("ItemTransformComponent");
		TextComponent* pText = (TextComponent*)pEntity->AddComponent("TextComponent");
		pText->SetText("hello, world");
		pTransform->SetLeft(100);
		pTransform->SetWidth(100);
		pTransform->SetHeight(100);
		pTransform->SetVisible(true);
		pMainEntity->AddChild(pEntity);
	}

	{
		YY::Entity* pEntity = (YY::Entity*)GetMgr()->Create("Entity");
		ItemTransformComponent* pTransform = (ItemTransformComponent*)pEntity->AddComponent("ItemTransformComponent");
		ImageComponent* pImage = (ImageComponent*)pEntity->AddComponent("ImageComponent");
		pImage->SetImageSource("ui_image/grass.bmp");
		pTransform->SetLeft(500);
		//pTransform->SetWidth(100);
		//pTransform->SetHeight(100);
		pTransform->SetVisible(true);
		pMainEntity->AddChild(pEntity);
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

