/************************************************************************/
/* 
@author:    junliang
@brief:     edit map
@time:      2017-6-16
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"


class IEditMap : public IComponent
{
public:
	virtual bool SetParams(int w, int h)=0;
	virtual bool RayPick(int x, int y, float& x_, float& y_, float& z_) = 0;
};