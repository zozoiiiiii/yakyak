/************************************************************************/
/* 
@author:    junliang
@brief:     edit axis
@time:      2017-6-16
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"


class IEditAxis : public IComponent
{
    enum 
    {
        SELECT_X,
        SELECT_Y,
        SELECT_Z,
    };
public:

    virtual bool RayPick(float x, float y, bool& selectX, bool& selectY, bool& selectZ) = 0;
    virtual void SetSelectX(bool value) = 0;
    virtual bool GetSelectX() const = 0;
    virtual void SetSelectY(bool value) = 0;
    virtual bool GetSelectY() const = 0;
    virtual void SetSelectZ(bool value) = 0;
    virtual bool GetSelectZ() const = 0;
};