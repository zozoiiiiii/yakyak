/************************************************************************/
/* 
@author:    junliang
@brief:     编辑用地图， 根据地图宽高，画一个矩形，用作鼠标trace 3d坐标的功能
@time:      2017-6-26
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/components/i_edit_map.h"
#include "yy_world/inc/i_world.h"
#include "yy_world/inc/i_scene.h"

struct vertex_line_t
{
	float x,y,z;
	unsigned int color;
};

class EditMap : public IEditMap
{
    YY_BEGIN(EditMap, "Entity");
    YY_END;
public:
    virtual void OnCreate();
    virtual void OnDestroy(){}
    virtual void OnRender(RenderContext* pCxt);

    virtual bool RayPick(int x, int y, float& x_, float& y_, float& z_);

	bool SetParams(int w, int h);
private:
    bool GetRayInfo(int x, int y, YY::Vec3f& orig, YY::Vec3f& direction);
    YY::Vec3f GetPos(int index);
private:
    vertex_line_t vexData[6];
    int m_w;
    int m_h;
    YY::IMesh* m_pMesh;
};
