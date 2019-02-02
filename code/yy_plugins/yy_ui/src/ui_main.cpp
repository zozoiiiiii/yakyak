/************************************************************************/
/*
@author:  junliang
@brief:   ui framework

2d: form
3d: scene


entity------transform
	  ------sprite
      ------label
	  ------text

buttom control= entity(transform,sprint,button)
                    entity(transform,label)


@time:    2018/09/28
*/
/************************************************************************/

#include "yy_core.h"
#include "gui.h"
#include "rectangle_component.h"
#include "rectangle_render_component.h"
#include "item_transform_component.h"
#include "positioner/column_component.h"
#include "positioner/row_component.h"


//#include "views/scroll_bar.h"
//#include "views/scroll_view.h"
//#include "views/scroll_view_delegate.h"
//#include "views/scroll_view_model.h"
//#include "views/tree_view_model.h"
//#include "views/tree_view.h"
//#include "button.h"
//#include "mouse_area.h"

//#include "item_generator_component.h"
//#include "anchor_fill_component.h"
//#include "text.h"



void plugin_main_ui(YY::IObjectMgr* pEntMgr)
{
    SetGlobalEntMgr(pEntMgr);

	YY_REG(IGUI);
	YY_REG(GUI);
	YY_REG(ItemTransformComponent);
	YY_REG(RectangleComponent);
	YY_REG(RectangleRenderComponent);

	//YY_REG(ColumnComponent);
	//YY_REG(RowComponent);

	//YY_REG(Image);
	//YY_REG(Text);

//	YY_REG(ItemGeneratorComponent);
//	YY_REG(AnchorFillComponent);

	//YY_REG(Button);
	//YY_REG(MouseArea);
	//YY_REG(ScrollBar);
	//YY_REG(ScrollView);
	//YY_REG(ScrollViewDelegate);
	//YY_REG(ScrollViewModel);
	//YY_REG(TreeViewModel);
	//YY_REG(TreeView);

}
