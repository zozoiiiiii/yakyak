/************************************************************************/
/* 
@author:    junliang
@brief:     line
@time:      2017-6-16
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"
#include "yy_render/inc/model/i_mesh.h"


class Cube : public GameObjComponent
{
    YY_BEGIN(Cube, GameObjComponent);
	YY_METHOD(&Cube::SetColor, "SetColor", "");
	YY_METHOD(&Cube::Init, "Init", "");
    YY_FIELD(&Cube::m_width, "width", "");
	YY_FIELD(&Cube::m_height, "height", "");
	YY_FIELD(&Cube::m_depth, "depth", "");
    YY_END;
public:
    virtual void OnCreate();
    virtual void OnDestroy();
	virtual void OnRender(IRender* pRender, RenderContext* pCxt);

	void Init(int width, int height, int depth);
    void SetColor(sint32 color);
private:
    void SetupMesh();
private:
    YY::IMesh* m_pMesh;
    int m_width;
    int m_height;
    int m_depth;
    unsigned int m_color;
};