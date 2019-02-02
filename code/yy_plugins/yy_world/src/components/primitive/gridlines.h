/************************************************************************/
/* 
@author:    junliang
@brief:     grid lines component
@time:      2017-6-16
*/
/************************************************************************/
#pragma once
#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

class GridLines : public IComponent
{
    YY_BEGIN(GridLines, "Entity");
    YY_END

    struct vertex_line_t
    {
        float x,y,z;
        unsigned int color;
    };
public:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnRender(RenderContext* pCxt);
private:
#define MAX_VERTEXT_COUNT 80
    vertex_line_t vexData[MAX_VERTEXT_COUNT];

    unsigned int VBO;
};