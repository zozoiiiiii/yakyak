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


class Lines : public IComponent
{
    YY_BEGIN(Lines, "BaseObject");
    YY_METHOD(&Lines::DrawLine, "DrawLine", "");
    YY_END
    struct vertex_line_t
    {
        YY::Vec3f pos;
        unsigned int color;
    };
public:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnRender(RenderContext* pCxt);

    void DrawLine(const YY::Vec3f& from, const YY::Vec3f& to, unsigned int color);
    void DrawSingleLine(const YY::Vec3f& from, const YY::Vec3f& to, unsigned int color);
private:
#define MAX_VERTEXT_COUNT 80
    vertex_line_t m_vexData[MAX_VERTEXT_COUNT];
    int m_nLineNum;

    unsigned int m_VBO;
};