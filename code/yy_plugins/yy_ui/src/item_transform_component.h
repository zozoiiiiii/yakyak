/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/09/28
*/
/************************************************************************/
#pragma once

#include "core/inc/yy_entity.h"


// texture offset
//#define SETTEXX(x) ((float(x) - 0.5f)) 
//#define SETTEXY(y) (-(float(y) -0.5f))

// point offset, normally, (0,0) is in the right,left, so need to change to top,left
//#define SETX(x)	((x-0.5f))
//#define SETY(y)	(-(y-0.5f))


//#define COLOR_ARGB(a, r, g, b) ((unsigned int)((((a) & 0xFF)<<24) \
//	| (((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF)))

//#define COLOR_RGBA(r, g, b, a) COLOR_ARGB(a, r, g, b)

// color: a r g b
#define COLOR_RED 0xFFFF0000
#define COLOR_GREE 0xFF00FF00
#define COLOR_BLUE 0xFF0000FF
#define COLOR_EDITOR 0xFF535353
#define COLOR_TREEITEM_HIGHLIGHT 0xFF3A3A3A

NS_YY_BEGIN
class ItemTransformComponent : public Component
{
	YY_BEGIN(ItemTransformComponent, Component);
	YY_FIELD(&ItemTransformComponent::m_top, "top", "");
	YY_FIELD(&ItemTransformComponent::m_left, "left", "");
	YY_FIELD(&ItemTransformComponent::m_width, "width", "");
	YY_FIELD(&ItemTransformComponent::m_height, "height", "");
	//YY_FIELD(&Item::m_pAnchorLayout, "anchor", "");
	YY_END
public:
	ItemTransformComponent();
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroyed();


	void SetLeft(float left);
	void SetAbsLeft(float left);
	float GetLeft();
	float GetAbsLeft();

	void SetTop(float top);
	void SetAbsTop(float top);
	float GetTop();
	float GetAbsTop();
	ItemTransformComponent* FindParentTransform();


	float GetWidth();
	void SetWidth(float w);

	float GetHeight();
	void SetHeight(float h);

	int GetOpacity();
	void SetOpacity(int opacity);
	bool GetVisible();
	void SetVisible(bool visible);

	void SetTransformChanged(bool bChanged=true);
	bool IsTransformChanged() { return m_bTransformChanged; }
private:
	void Show();
	void Close();
	void ShowModal();
	void CloseModal();
private:
	float m_top;
	float m_left;
	float m_width;
	float m_height;
	int m_opacity;
	bool m_visible;
	bool m_bTransformChanged;
	//Component* m_pAnchorLayout;
};

NS_YY_END