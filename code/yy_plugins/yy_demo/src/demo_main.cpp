/************************************************************************/
/*
@authro:    junliang
@brief:     main
@time:      20140404
*/
/************************************************************************/
#include "yy_core.h"
#include "demo.h"
//#include "views/hierarchy_view.h"
//#include "views/property_view.h"
//#include "views/design_view.h"

void plugin_main_demo(YY::IObjectMgr* pEntMgr)
{
	IReflectionMgr* pReflectionMgr = pEntMgr->GetReflectionMgr();
	YY_REG(pReflectionMgr, Demo);
	//YY_REG(HierarchyView);
	//YY_REG(Design);

}
