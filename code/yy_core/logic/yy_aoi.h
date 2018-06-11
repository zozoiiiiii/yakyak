/************************************************************************/
/* 
@author:    junliang
@brief:     场景可视范围管理, area of interest.
@time:      5/23/2016
*/
/************************************************************************/
#pragma once
#include "entity/inc/yy_entity.h"
#include "FastDelegate.h"
#include <map>

NS_YY_BEGIN

#define scene_width 10000                //场景宽
#define scene_height 10000                //场景高
#define scene_gride_distance 1000        //场景可视网格边长

// 固定aoi半径
#define visual_range                1000

// aoi用到的实体信息:位置
struct aoi_obj
{
    float x;
    float y;
};

enum aoiflag
{
    aoi_scene,                //场景内的aoi事件
    aoi_switch_scene,        //切场景引发的aoi事件
};

//网格
struct mapblock
{
    std::map<YY_ENTITYID, aoi_obj> obj_set;        //该网格内的所有实体
};

//参数4是AOIFLAG, 作用是离开事件,如果是离开场景, 则不要发送给mover消息
typedef fastdelegate::FastDelegate3<const YY_ENTITYID&, const YY_ENTITYID&, int> cb_enter;
typedef fastdelegate::FastDelegate3<const YY_ENTITYID&, const YY_ENTITYID&, int> cb_leave;
typedef fastdelegate::FastDelegate4<const YY_ENTITYID&, const YY_ENTITYID&, float, float> cb_move;

class aoi
{
public:
    aoi(cb_enter onenter, cb_leave onleave, cb_move onmove);
    ~aoi();

    //把地图分割成多行多列的正方形网格, 网格边长必须大于等于实体的可视半径
    void init(int row, int column, int distance);
    void enter(const YY_ENTITYID& obj, float x, float y);
    void leave(const YY_ENTITYID& obj, float x, float y);

    //移动到目标点, 导致触发的aoi事件
    void moveto(const YY_ENTITYID& obj, float x, float y);

    //获得可视列表
    void get(float x, float y, int distance, std::vector<YY_ENTITYID>& v);
private:
    //把指定网格内的实体集合追加到集合内
    void append(float x, float y, int distance, int index, std::vector<YY_ENTITYID>& v);
    mapblock* get_mapblock(float x, float y);
private:
    int m_row;
    int m_column;
    int m_distance;                            //正方形网格边长
    mapblock* m_blocks;                        //所有网格

    //aoi回调
    cb_enter m_onenter;
    cb_leave m_onleave;
    cb_move m_onmove;
};
NS_YY_END