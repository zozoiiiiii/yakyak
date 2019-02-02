#include "app.h"
#include "dll_mgr.h"
#include "yy_core.h"
#include "yy_render/inc/i_render.h"
//#include "yy_world/inc/i_world.h"
#include "yy_ui/inc/i_gui.h"



extern void plugin_main_demo(YY::IObjectMgr* pEntMgr);
extern void plugin_main_render(YY::IObjectMgr* pEntMgr);
//extern void plugin_main_world(YY::IObjectMgr* pEntMgr);
extern void plugin_main_ui(YY::IObjectMgr* pEntMgr);

App::App()
:m_bOpened(false), m_pEntMgr(NULL)
{
	m_pEntMgr = CreateEntMgr();
	SetGlobalEntMgr(m_pEntMgr);
}

App* App::Instance()
{
    static App s_app;
    return &s_app;
}

void App::Open(float width, float height, const std::string& resPath)
{

	plugin_main_demo(m_pEntMgr);
	plugin_main_render(m_pEntMgr);
	plugin_main_ui(m_pEntMgr);

	if(!resPath.empty())
		setPath(resPath);

    //try{

        // load dll
        //std::string config_name = "loader.xml";
        //std::string file = YY::ExePath() + "\\" + config_name.c_str();
        //DllMgr* pDllMgr = new DllMgr;
        //pDllMgr->Load(m_pEntMgr, file.c_str());

	IRender* pRender = IRender::Instance();
        pRender->SetResPath(resPath);
        pRender->SetWinWidth(width);
        pRender->SetWinHeight(height);
        pRender->SetDeviceWidth(width);
        pRender->SetDeviceHeight(height);
        pRender->CreateDevice();
		IGUI::Instance()->ReSize(width, height);


        // create game
        m_pEntMgr->Create("Demo");
        m_bOpened = true;
        

    //}catch(YY::AssertFailureException& err)
    {
    }
}

void App::Close()
{
    
}

void App::Excute()
{
	if (!m_bOpened)
		return;

    if(!m_pEntMgr)
        return;

    //try{
        static sint64 lasttime = YY::TimeStamp();
        static sint64 curtime = lasttime;
        static sint64 detla = 0;        // micro seconds
        static float fdetla = 0.0f;    // seconds
        curtime = YY::TimeStamp();
        detla = curtime - lasttime;
        fdetla = (float)(detla) / 1000.0f;
        fdetla /= 1000.0f;
        m_pEntMgr->OnExcute(fdetla);
        lasttime = curtime;
   // }
   // catch (YY::AssertFailureException& err)
    //{
        return;
    //}
}


bool App::OnMsg(unsigned int msg, size_t param1, size_t param2)
{
    if(!m_pEntMgr)
        return false;

	return m_pEntMgr->OnMsg(msg, param1, param2);

    //try{
    //    if(msg==WM_LBUTTONDOWN)
        {
            //IWorld::Instance()->GetScene()->ScreenShot("c:/test1.jpg", 0,0,500,500);
        }

//        return m_pEntMgr->OnMsg(msg, param1, param2);
   // }catch(YY::AssertFailureException& err)
    {
        //YY_LOG_ERR(err.What());
    }

    return false;
}

void App::Resize(float width, float height)
{
	IGUI::Instance()->ReSize(width, height);
}