/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    2018/09/28
*/
/************************************************************************/
#pragma once
#include "yy_ui/inc/i_gui.h"
#include "yy_render/inc/i_render.h"
#include "yy_render/inc/i_batch.h"
#include "yy_render/inc/i_render_context.h"

NS_YY_BEGIN
class GUI : public IGUI
{
    YY_BEGIN(GUI, IGUI);
    YY_END
public:
	virtual void OnCreate(const VariantMap& args);
	virtual void OnDestroy();
	virtual void ReSize(float w, float h);


	YY_OBJECTID GetDesktopForm() const;
	void AddModalForm(YY_OBJECTID control);
	void RemoveModalForm(YY_OBJECTID control);
	virtual IRender* GetRender() { return m_pRender; }

private:
	void onEvent_AddBatch(const char* name, const YY::VarList& args);
	void visitEntities(Entity* pEntity);
private:
	IRender* m_pRender;
	IBatchGroup* m_pUIBatchGroup;
	RenderContext* m_pRenderContext;
	Entity* m_pDeskTop;	// canvas node
	std::vector<YY_OBJECTID> m_modalForms; // pop up form
};
NS_YY_END