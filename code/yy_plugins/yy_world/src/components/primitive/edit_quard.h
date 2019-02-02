/************************************************************************/
/* 
@author:    junliang
@brief:     �༭�������壬 fboʱ����ȫ����
@time:      2017-6-26
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"
#include "yy_world/inc/i_world.h"
#include "yy_world/inc/i_scene.h"
#include "yy_world/inc/i_render_context.h"

class EditQuard : public IComponent
{
    YY_BEGIN(EditQuard, "Entity");
    YY_END;
public:
    virtual void OnCreate();
    virtual void OnDestroy(){}
    virtual void OnRender(RenderContext* pCxt);

private:
    void SetUpMesh();
private:
    YY::IMesh* m_pMesh;
};
