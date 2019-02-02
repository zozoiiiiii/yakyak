/************************************************************************/
/* 
@author:    junliang
@brief:     skybox component

天空盒 = npc在立方体内部的即视感

help: 
https://github.com/wangdingqiao/noteForOpenGL/tree/master/skyBox
http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

how to create skybox texture:
http://www.leadwerks.com/werkspace/topic/14047-skybox-question/

@time:      11/28/2016
*/
/************************************************************************/
#pragma once

#include "render/inc/model/i_mesh.h"
#include "comp_camera.h"
#include "i_game_obj.h"


#define YY_COMP_SKYBOX_CREATE_NAME "Comp_SkyBox"

class Comp_SkyBox : public Component
{
    YY_BEGIN(Comp_SkyBox);
    YY_END
public:
    ~Comp_SkyBox(){};
    virtual void OnCreate(const YY::VarList& args);
    virtual void OnDestroy();
    virtual void OnRender(RenderContext* pCxt);

    void LoadTex(const std::string& PosXFilename,
        const std::string& NegXFilename,
        const std::string& PosYFilename,
        const std::string& NegYFilename,
        const std::string& PosZFilename,
        const std::string& NegZFilename);

private:
    void CreateMesh();

private:
    int m_nTexID_UP;
    int m_nTexID_Side;
    int m_nTexID_Cube;
    YY::SubMesh* m_pSubMesh;
    Comp_Camera* m_pComp_Camera;
};