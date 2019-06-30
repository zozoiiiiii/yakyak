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
#include "yy_world/inc/i_game_obj.h"
#include "yy_world/inc/i_world.h"
#include "yy_world/inc/components/i_comp_mesh_render.h"


using namespace YY;
void Demo::OnCreate()
{
	CreateTestUI();
	CreateTestScene();




	BaseObject* pNewItem = BaseObject::parseFromFile(GetMgr(),  "D:\\projects\\yakyak\\bin\\res\\ui_layout\\test.json");
	Item* pItem = (Item*)GetMgr()->Create("Item");
	Item* pItem1 = (Item*)GetMgr()->Create("Item");
	pItem->AddChild(pItem1);

	pItem->serializeToFile("D:\\projects\\yakyak\\bin\\res\\ui_layout\\test.json");
	//BaseObject* pObject = BaseObject::parseFromFile(m_pEntMgr, "D:\\work\\projects\\fun\\yakyak\\bin\\res\\ui_layout\\test.json");

}

void Demo::CreateTestUI()
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
		pItem->SetWidth(100);
		pItem->SetHeight(100);
		pItem->SetVisible(true);
		pMainEntity->AddChild(pItem);
	}

	pMainEntity->serializeToFile("ttt.xml");



	// create gui and render module
	//IGUI::Instance(GetMgr());
	//MainView* pMainView = new MainView(GetMgr());
	return;
// 	float width = IRender::Instance(GetMgr())->GetDeviceWidth();
// 	float height = IRender::Instance(GetMgr())->GetDeviceHeight();
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



void Demo::CreateTestScene()
{
	IScene* pScene = (IScene*) GetMgr()->Create("Scene");
	if (!pScene)
		return;

	int width = IRender::Instance(GetMgr())->GetDeviceWidth();
	int height = IRender::Instance(GetMgr())->GetDeviceHeight();

	std::string res_path = IRender::Instance(GetMgr())->GetResPath();


	{
// 		IGameObj* pObj = pScene->CreateObj();
// 		pObj->SetName("GameObj_Cube");
// 		pScene->AddObj(pObj);
// 		pObj->SetReceiveShadow(true);
// 		Component* pComponent = pObj->AddComponent("Cube");
// 		int width = 1;
// 		int height = 1;
// 		int depth = 1;
// 		pComponent->Invoke(NULL, "Init", &width, &height, &depth);
// 
// 		YY::Vec3f from(0, 0, 0);
// 		YY::Vec3f to(10, 0, 0);
// 		int color = 0xFFFF0000;
// 		pComponent->Invoke(NULL, "SetColor", &color);
// 
// 		pObj->GetTransform()->SetPos(3, 0, 0);
	}


	// camera
	{
		IGameObj* pObj_Camera = pScene->CreateObj();
		pObj_Camera->SetName("Camera");
		pScene->SetMainCamera(pObj_Camera);

		pObj_Camera->GetTransform()->SetPos(0.0f, 2.0f, 10.0f);
		pObj_Camera->GetTransform()->SetAngle(0.0f, 0.0f, 0.0f);
		pObj_Camera->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

		Component* pComp_Camera = pObj_Camera->AddComponent("Comp_Camera");

		float aspect = (float)width / (float)height;
		//pComp_Camera->GetMetaClass()->methods->SetPersp(90.0f, aspect, 0.1f, 100.0f);
		float fov = 45.0f;
		float nearZ = 0.1f;
		float farZ = 1000.0f;
		pComp_Camera->Invoke(NULL, "SetPersp", &fov, &aspect, &nearZ, &farZ);

		// skybox
		//           Comp_SkyBox* pComp_SkyBox = (Comp_SkyBox*)pObj_Camera->AddComp(YY_COMP_SKYBOX_CREATE_NAME);
		//           pComp_SkyBox->LoadTex(
		//               get_res_path()+"texture/sky/cubemap_posx.jpg",
		//               get_res_path()+"texture/sky/cubemap_negx.jpg",
		//               get_res_path()+"texture/sky/cubemap_posy.jpg",
		//               get_res_path()+"texture/sky/cubemap_negy.jpg",
		//               get_res_path()+"texture/sky/cubemap_posz.jpg",
		//               get_res_path()+"texture/sky/cubemap_negz.jpg");
	}

	// 	{
	// 		IGameObj* pObj_Camera = m_pScene->CreateObj();
	// 		pObj_Camera->SetName("Camera2");
	// 		m_pScene->AddObj(pObj_Camera);
	// 
	// 		pObj_Camera->GetTransform()->SetPos(0.0f, 2.0f, 10.0f);
	// 		pObj_Camera->GetTransform()->SetAngle(0.0f,0.3f,0.0f);
	// 		pObj_Camera->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
	// 
	// 		IComponent* pComp_Camera = pObj_Camera->AddComp("Comp_Camera");
	// 
	// 		float aspect = (float) width / (float) height;
	// 		//pComp_Camera->GetMetaClass()->methods->SetPersp(90.0f, aspect, 0.1f, 100.0f);
	// 		float fov = 45.0f;
	// 		float nearZ = 0.1f;
	// 		float farZ = 1000.0f;
	// 		pComp_Camera->Invoke(NULL, "SetPersp", &fov, &aspect, &nearZ, &farZ);
	// 	}

	
	// terrain
	{
 		IGameObj* pObj_Terrain = pScene->CreateObj();
 		pObj_Terrain->SetName("Terrain");
 		pScene->AddObj(pObj_Terrain);
 		//m_pScene->SetTerrain(pObj_Terrain);
 		pObj_Terrain->SetReceiveShadow(true);
 		pObj_Terrain->SetCastShadow(true);
 
 		pObj_Terrain->GetTransform()->SetPos(0.0f, 0.0f, 0.0f);
 		pObj_Terrain->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
 
 		Component* pComp_Terrain = pObj_Terrain->AddComponent("Comp_Terrain");
 		//pComp_Terrain->LoadTex(res_path+"texture/terrain/terrain.jpg", res_path+"texture/terrain/sand.jpg");
 		std::string high_map = "texture/terrain/terrain.jpg";
 		std::string tex = "texture/terrain/sand.jpg";
 		pComp_Terrain->Invoke(NULL, "LoadTex", &high_map, &tex);
	}

	// player
	{
 		IGameObj* pObj_Npc = pScene->CreateObj();
 		pObj_Npc->SetName("Player");
 		pScene->AddObj(pObj_Npc);
 		pObj_Npc->SetReceiveShadow(true);
 		pObj_Npc->SetCastShadow(true);
 
 		pObj_Npc->GetTransform()->SetPos(0.0f, 0.0f, 0.0f);
 		pObj_Npc->GetTransform()->SetAngle(0.0f, 0.0f, 0.0f);
 		pObj_Npc->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
 
 		IComp_MeshRender* pComp_MeshRender = (IComp_MeshRender*)pObj_Npc->AddComponent("Comp_MeshRender");
 
 		std::string Model_path = "models/hero/";
 		std::string Mesh_file = "hero.yym";
 		std::string Skeleton_file = "hero.yys";
 		pComp_MeshRender->Invoke(NULL, "SetMesh", &Model_path, &Mesh_file, &Skeleton_file);
 		int nAnim1 = pComp_MeshRender->AddAnim("stand.yya");
 		int nAnim2 = pComp_MeshRender->AddAnim("run.yya");
 		pComp_MeshRender->SetDefaultAnim(nAnim1);

		// test serialize
		//pObj_Npc->SerializeToFile(YY::ExePath() + "/test.xml");
	}

	// quad : show the depth map
	{
// 		IGameObj* pObj = m_pScene->CreateObj();
// 		pObj->SetName("ShadowMapDepth");
// 		m_pScene->AddObj(pObj);
// 
// 		std::string modelName = res_path + "models/quad/quad.obj";
// 		std::string modelDir = res_path + "models/quad/";
// 
// 		IShowDepthMap* pComp_Model = (IShowDepthMap*)pObj->AddComponent("ShowDepthMap");
// 		pComp_Model->Load(modelName, modelDir);
// 
// 		pObj->GetTransform()->SetPos(3.0f, 2.0f, -1.0f);
// 		pObj->GetTransform()->SetScale(1, 1, 1);
	}
}