/************************************************************************/
/* 
@author:    junliang
@brief:     �������ӷ�Χ����, area of interest.
@time:      5/23/2016
*/
/************************************************************************/
#pragma once
#include "entity/inc/yy_entity.h"
#include "FastDelegate.h"
#include <map>

NS_YY_BEGIN

#define scene_width 10000                //������
#define scene_height 10000                //������
#define scene_gride_distance 1000        //������������߳�

// �̶�aoi�뾶
#define visual_range                1000

// aoi�õ���ʵ����Ϣ:λ��
struct aoi_obj
{
    float x;
    float y;
};

enum aoiflag
{
    aoi_scene,                //�����ڵ�aoi�¼�
    aoi_switch_scene,        //�г���������aoi�¼�
};

//����
struct mapblock
{
    std::map<YY_ENTITYID, aoi_obj> obj_set;        //�������ڵ�����ʵ��
};

//����4��AOIFLAG, �������뿪�¼�,������뿪����, ��Ҫ���͸�mover��Ϣ
typedef fastdelegate::FastDelegate3<const YY_ENTITYID&, const YY_ENTITYID&, int> cb_enter;
typedef fastdelegate::FastDelegate3<const YY_ENTITYID&, const YY_ENTITYID&, int> cb_leave;
typedef fastdelegate::FastDelegate4<const YY_ENTITYID&, const YY_ENTITYID&, float, float> cb_move;

class aoi
{
public:
    aoi(cb_enter onenter, cb_leave onleave, cb_move onmove);
    ~aoi();

    //�ѵ�ͼ�ָ�ɶ��ж��е�����������, ����߳�������ڵ���ʵ��Ŀ��Ӱ뾶
    void init(int row, int column, int distance);
    void enter(const YY_ENTITYID& obj, float x, float y);
    void leave(const YY_ENTITYID& obj, float x, float y);

    //�ƶ���Ŀ���, ���´�����aoi�¼�
    void moveto(const YY_ENTITYID& obj, float x, float y);

    //��ÿ����б�
    void get(float x, float y, int distance, std::vector<YY_ENTITYID>& v);
private:
    //��ָ�������ڵ�ʵ�弯��׷�ӵ�������
    void append(float x, float y, int distance, int index, std::vector<YY_ENTITYID>& v);
    mapblock* get_mapblock(float x, float y);
private:
    int m_row;
    int m_column;
    int m_distance;                            //����������߳�
    mapblock* m_blocks;                        //��������

    //aoi�ص�
    cb_enter m_onenter;
    cb_leave m_onleave;
    cb_move m_onmove;
};
NS_YY_END