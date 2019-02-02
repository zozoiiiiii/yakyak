/************************************************************************/
/* 
@author:    junliang
@brief:     edit axis

1�� ��������ʱ����߱����Ǵ��ڿ��
2�� ���԰����߻�������������

http://antongerdelan.net/opengl/raycasting.html
http://antongerdelan.net/opengl/raycasting.html


@time:      2017-6-16
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"
#include "yy_world/inc/components/i_edit_axis.h"


class EditAxis : public IEditAxis
{
    YY_BEGIN(EditAxis, "Entity");
    YY_END;
    enum 
    {
        SELECT_X = 0x01,
        SELECT_Y = 0x02,
        SELECT_Z = 0x04,
    };
    struct vertex_line_t
    {
        YY::Vec3f pos;
        unsigned int diffuse;
    };
public:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnAddRender(IBatchMgr* pBatchMgr);
    virtual void OnRender(RenderContext* pCxt);

    bool RayPick(float x, float y, bool& selectX, bool& selectY, bool& selectZ);

    void SetSelectX(bool value);
    bool GetSelectX() const;
    void SetSelectY(bool value) ;
    bool GetSelectY() const ;
    void SetSelectZ(bool value) ;
    bool GetSelectZ() const ;

private:
    void InitVertex();
    void DrawAxisLine(RenderContext* pCxt);
    void DrawAxisPick(RenderContext* pCxt);
private:
	vertex_line_t m_LineVertex[6];
	vertex_line_t m_PickVertex[18 * 3];

    unsigned int m_vbo_line;
    unsigned int m_vbo_pick;
    bool m_bSelectX;
    bool m_bSelectY;
    bool m_bSelectZ;
    unsigned int m_nPickTriangleNum;        // ��ͷ����������������
    unsigned int m_nPickFlags[18];          // һ����ͷ�ĸ��棬 3����ͷ12���棬����xz,yz,xy��6���棬�ܹ�18���档ÿ�����¼��Ӧ�������

    bool m_bShowPlaneXY;
    bool m_bShowPlaneXZ;
    bool m_bShowPlaneYZ;
};