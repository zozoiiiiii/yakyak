/************************************************************************/
/*
@author:  junliang
@brief:

screen(LCD) coordinate( we use here)
0,0----------x+
|
|
|
y+



cartesian coordinate

y+
|
|
|
0,0-------------x+

ui in game:  render every frame
ui in desktop application: render when data dirty

///////////////////////////////////////////////
cocos creator:
	gulp, grunt
	----------
	react
	----------
	electron
cocos2dx project:
	json+javascript
android:
	xml+java
wpf:
	xaml+c#



////////////////////my design///////////////
// xml
<item ui-class="Item">
	<item class="Button" name="TestButton">hello</item>
	<item class="MouseArea" name="clickArea" anchorFill="TestButton">
</item>

// c++
// lua
// javascript
onCreated()
{
	var msArea = findChild("clickArea");
	msArea.bind("click", onButtonClicked);
}

onDestroyed(){}
onExecute(float seconds){}
onButtonClicked(){}




///////////////////////////////////////////////
how to describ one view: use xml best
1. xml, html5+css(electron)
2. json
3. qml
4. c++


1. xml
<ui:group layout="vertial">
  <ui:block width="200" layout="horizontal">
	<ui:input value="Search"></ui:input>
	<ui:button>Search</ui:button>
  </ui:block>
  <ui:block width="400"></ui:block>
</ui:group>

2. json
{
  "type": "ui:group",
  "layout": "vertical",
  "children": [
	{
	  "type": "ui:block",
	  "width": 200,
	  "layout": "horizontal",
	  "children": [
		{ "type": "ui:input", "value": "search" },
		{ "type": "ui:button", "label": "Search" },
	  ]
	},
	{
	  "type": "ui:block",
	  "width": 400,
	},
  ],
}

3. qml
Window {
	visible: true
	width: 640
	height: 480
	title: qsTr("Hello World")

	Media{
		anchors.fill: parent
	}
}

4. c++
Item* pForm = (Item*)GetMgr()->Create("Item");
pForm->SetWidth(100);
pForm->SetHeight(height);
YY::Rectangle* pRectangle = (YY::Rectangle*)GetMgr()->Create("Rectangle");
pRectangle->SetAbsLeft(width-pForm->GetWidth());
pRectangle->SetAbsTop(0);
pRectangle->SetWidth(400);
pRectangle->SetColor(COLOR_GREE);
pForm->AddChild(pRectangle);
pForm->ShowModal();


reference: 
1. Johnny Wu, the cocos creator writer
   consider javascript as the cocos2dx engine script(can be web app, can use the brower)
   use electron in order to render the ui by cocos creator
   one xml as one view.



@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "core/inc/yy_entity.h"

class IRender;

NS_YY_BEGIN
class IGUI : public BaseObject
{
	YY_VIRTUAL_BEGIN(IGUI, BaseObject);
	YY_END
public:
	static IGUI* Instance(IObjectMgr* pObjMgr)
	{
		static IGUI* s_pGUI = nullptr;
		if (s_pGUI)
			return s_pGUI;

		BaseObject* pObject = pObjMgr->FindGlobalObject("GUI");
		if (NULL == pObject)
		{
			BaseObject* pBaseObject = pObjMgr->Create("GUI");
			pObjMgr->SetGlobal("GUI", pBaseObject->GetID());
			pObject = pBaseObject;
		}

		s_pGUI = (IGUI*)pObject;
		return s_pGUI;
	}

	virtual YY_OBJECTID GetDesktopForm() const = 0;
	virtual void AddModalForm(YY_OBJECTID control) = 0;
	virtual void RemoveModalForm(YY_OBJECTID control) = 0;
	virtual void ReSize(float w, float h) = 0;
	virtual IRender* GetRender() = 0;
};
NS_YY_END