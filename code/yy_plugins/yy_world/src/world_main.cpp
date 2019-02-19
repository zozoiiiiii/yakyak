/************************************************************************/
/*
@authro:    junliang
@brief:     main
@time:      20140404
*/
/************************************************************************/


#include "yy_core.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/octree_scene.h"
#include "scene/shadow_map.h"

// #include "components/primitive/lines.h"
// #include "components/primitive/gridlines.h"
// #include "components/primitive/cube.h"
// #include "components/primitive/edit_axis.h"
// #include "components/primitive/edit_quard.h"


#include "components/terrain/comp_terrain.h"
#include "components/mesh_render/comp_mesh_render.h"
#include "components/camera/comp_camera.h"
//#include "components/model_obj/comp_model_obj.h"
//#include "components/model_obj/show_depth_map.h"
//#include "components/model_obj/quad.h"
#include "gameobj/game_obj.h"
#include "gameobj/transform.h"
//#include "event/event_mgr.h"

//#include "components/primitive/edit_map.h"

//#include "scene/camera_view.h"


void plugin_main_world(YY::IObjectMgr* pEntMgr)
{
    SetGlobalEntMgr(pEntMgr);
    // yy_core begin
    YY_REG(Vec3f);

    // yy_core end.

    YY_REG(World);
    YY_REG(Scene);
    YY_REG(OctreeScene);
    YY_REG(ShadowMap);
	//YY_REG(CameraView);
    YY_REG(Weather);

    // primitive
//     YY_REG(GridLines);
//     YY_REG(Lines);
//     YY_REG(Cube);
//     YY_REG(EditQuard);

    //YY_REG(Comp_Terrain);
    YY_REG(Comp_MeshRender);
    YY_REG(Comp_Camera);
//    YY_REG(Comp_Model_Obj);
    //YY_REG(ShowDepthMap);
    YY_REG(GameObj);
	YY_REG(GameObjComponent);
	YY_REG(Transform);
}
