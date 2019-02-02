#include "scene_mgr.h"

#include "yy_world/inc/components/i_comp_camera.h"
#include "yy_world/inc/components/i_comp_mesh_render.h"
#include "yy_world/inc/components/i_show_depth_map.h"

void SceneMgr::OnCreate()
{
	m_pScene = IWorld::Instance()->GetScene();
}

void SceneMgr::OnDestroy()
{
    
}

void SceneMgr::OnExcute(float sec)
{
    
}


void SceneMgr::EnterScene(const char* scene_file)
{
    throw_assert(false, "not support now");
}

void SceneMgr::EnterEmptyScene()
{
    int width = m_pScene->GetRender()->GetDeviceWidth();
    int height = m_pScene->GetRender()->GetDeviceHeight();
    
    std::string res_path = m_pScene->GetRender()->GetResPath();


    {
//         IGameObj* pObj = m_pScene->CreateObj();
//         pObj->SetName("GameObj_Cube");
//         m_pScene->AddObj(pObj);
//         pObj->SetReceiveShadow(true);
//         IComponent* pComponent = pObj->AddComp("Cube");
// 		int width=1;
// 		int height=1;
// 		int depth=1;
// 		pComponent->Invoke(NULL, "Init", &width, &height, &depth);
// 
//         YY::Vec3f from(0,0,0);
//         YY::Vec3f to(10,0,0);
//         int color = 0xFFFF0000;
//         pComponent->Invoke(NULL, "SetColor", &color);
// 
//         pObj->GetTransform()->SetPos(3,0,0);
    }

    // camera
    {
        IGameObj* pObj_Camera = m_pScene->CreateObj();
        pObj_Camera->SetName("Camera");
        m_pScene->SetMainCamera(pObj_Camera);

        pObj_Camera->GetTransform()->SetPos(0.0f, 2.0f, 10.0f);
        pObj_Camera->GetTransform()->SetAngle(0.0f,0.0f,0.0f);
        pObj_Camera->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

        IComponent* pComp_Camera = pObj_Camera->AddComp("Comp_Camera");

        float aspect = (float) width / (float) height;
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
          IGameObj* pObj_Terrain = m_pScene->CreateObj();
          pObj_Terrain->SetName("Terrain");
          m_pScene->AddObj(pObj_Terrain);
  		//m_pScene->SetTerrain(pObj_Terrain);
          pObj_Terrain->SetReceiveShadow(true);
          pObj_Terrain->SetCastShadow(true);
  
          pObj_Terrain->GetTransform()->SetPos(0.0f, 0.0f, 0.0f);
          pObj_Terrain->GetTransform()->SetScale(1.0f,1.0f,1.0f);
  
          IComponent* pComp_Terrain = pObj_Terrain->AddComp("Comp_Terrain");
          //pComp_Terrain->LoadTex(res_path+"texture/terrain/terrain.jpg", res_path+"texture/terrain/sand.jpg");
          std::string high_map = "texture/terrain/terrain.jpg";
          std::string tex = "texture/terrain/sand.jpg";
          pComp_Terrain->Invoke(NULL, "LoadTex", &high_map, &tex);
    }

    // player
    {
        IGameObj* pObj_Npc = m_pScene->CreateObj();
        pObj_Npc->SetName("Player");
        m_pScene->AddObj(pObj_Npc);
        pObj_Npc->SetReceiveShadow(true);
        pObj_Npc->SetCastShadow(true);

        pObj_Npc->GetTransform()->SetPos(0.0f, 0.0f, 0.0f);
        pObj_Npc->GetTransform()->SetAngle(0.0f, 0.0f, 0.0f);
        pObj_Npc->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

        IComp_MeshRender* pComp_MeshRender = (IComp_MeshRender*)pObj_Npc->AddComp("Comp_MeshRender");

        std::string Model_path =  "models/hero/";
        std::string Mesh_file = "hero.yym";
        std::string Skeleton_file = "hero.yys";
        pComp_MeshRender->Invoke(NULL, "SetMesh", &Model_path, &Mesh_file, &Skeleton_file);
        int nAnim1=pComp_MeshRender->AddAnim("stand.yya");
        int nAnim2=pComp_MeshRender->AddAnim("run.yya");
        pComp_MeshRender->SetDefaultAnim(nAnim1);

		// test serialize
		//pObj_Npc->SerializeToFile(YY::ExePath() + "/test.xml");
    }

    // quad : show the depth map
    {
//         IGameObj* pObj = m_pScene->CreateObj();
//         pObj->SetName("ShadowMapDepth");
//         m_pScene->AddObj(pObj);
// 
//         std::string modelName = res_path + "models/quad/quad.obj";
//         std::string modelDir = res_path + "models/quad/";
// 
//         IShowDepthMap* pComp_Model = (IShowDepthMap*)pObj->AddComp("ShowDepthMap");
//         pComp_Model->Load(modelName, modelDir);
// 
//         pObj->GetTransform()->SetPos(3.0f, 2.0f, -1.0f);
//         pObj->GetTransform()->SetScale(1,1,1);
    }

    // gui
    {
       /* IGameObj* pObj = m_pScene->CreateObj();
        pObj->SetName("GuiTest");
        m_pScene->AddObj(pObj);

        IComponent* pRectangle = pObj->AddComp("Rectangle");
        pObj->AddComp("MouseArea");
		*/
    }


}