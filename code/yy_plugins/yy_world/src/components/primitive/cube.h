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
#include "render/inc/model/i_mesh.h"


class Cube : public IComponent
{
    YY_BEGIN(Cube, "Component");
	YY_METHOD(&Cube::SetColor, "");
	YY_METHOD(&Cube::Init, "");
    YY_FIELD(&Cube::m_width, "width", "");
	YY_FIELD(&Cube::m_height, "height", "");
	YY_FIELD(&Cube::m_depth, "depth", "");
    YY_END;
public:
    virtual void OnCreate();
    virtual void OnDestroy();
	virtual void OnRender(RenderContext* pCxt);
	virtual bool ParseFrom(const YY::VarList& args, int& read_index);

	void Init(int width, int height, int depth);
    void SetColor(unsigned int color);
private:
    void SetupMesh();
private:
    YY::IMesh* m_pMesh;
    int m_width;
    int m_height;
    int m_depth;
    unsigned int m_color;
};