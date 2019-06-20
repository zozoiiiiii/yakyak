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
#include "item.h"
#include "rectangle_component.h"
#include "text_component.h"
#include "image_component.h"
//#include "positioner/column_component.h"
//#include "positioner/row_component.h"


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
	IReflectionMgr* pReflectionMgr = pEntMgr->GetReflectionMgr();
	YY_REG(pReflectionMgr, IGUI);
	YY_REG(pReflectionMgr, GUI);
	YY_REG(pReflectionMgr, Item);
	YY_REG(pReflectionMgr, ItemComponent);
	YY_REG(pReflectionMgr, RectangleComponent);

	//YY_REG(ColumnComponent);
	//YY_REG(RowComponent);

	YY_REG(pReflectionMgr, ImageComponent);
	YY_REG(pReflectionMgr, TextComponent);

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
