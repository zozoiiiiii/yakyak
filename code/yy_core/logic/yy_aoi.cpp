#include "util/inc/yy_aoi.h"
#include <algorithm>
#include <math.h>
#include "base/yy_exception.h"

NS_YY_BEGIN

aoi::aoi(cb_enter onenter, cb_leave onleave, cb_move onmove)
{
    m_onenter=onenter;
    m_onleave=onleave;
    m_onmove=onmove;
}

aoi::~aoi()
{
    if(NULL!=m_blocks)
        delete []m_blocks;
}

//把地图分割成正方形网格, 网格边长必须大于等于所有实体的可视半径
void aoi::init(int row, int column, int distance)
{
    m_row=row;
    m_column=column;
    m_distance=distance;
    m_blocks=new mapblock[row*column];
}

void aoi::enter(const YY_ENTITYID& obj, float x, float y)
{
    //可视范围不能大于网格边长
    throw_assert(visual_range <= m_distance, "可视半径:"<<visual_range<<" 不能大于网格边长:" << m_distance);

    //根据坐标获得所在网格
    mapblock* mb=get_mapblock(x, y);
    throw_assert(mb!=NULL, x<<"-"<<y<<" is out of range.");

    //网格记录新实体
    aoi_obj ao;
    ao.x = x;
    ao.y = y;
    mb->obj_set[obj] = ao;

    //获得可视列表
    std::vector<YY_ENTITYID> v;
    get(x,y,visual_range, v);

    //遍历可视列表，如果可视节点是否能看到自己，则调用回调
    for(int i=0; i<(int)v.size(); i++)
    {
        const YY_ENTITYID& tmp_id=v[i];
        m_onenter(tmp_id, obj, aoi_switch_scene);
    }
}

void aoi::leave(const YY_ENTITYID& obj, float x, float y)
{
    //获得可视列表
    std::vector<YY_ENTITYID> v;
    get(x, y, visual_range, v);

    //遍历可视列表
    for(int i=0; i<(int)v.size(); i++)
    {
        YY_ENTITYID tmp_id=v[i];
        m_onleave(tmp_id, obj, aoi_switch_scene);
    }

    //根据坐标获得所在网格
    mapblock* mb=get_mapblock(x, y);
    std::map<YY_ENTITYID, aoi_obj>::iterator itor = mb->obj_set.find(obj);
    if(itor == mb->obj_set.end())
    {
        return;
    }

    mb->obj_set.erase(itor);
}

void aoi::moveto(const YY_ENTITYID& obj, float x, float y)
{
    //获得旧坐标的可视列表old_set
    std::vector<YY_ENTITYID> old_set;
    get(x,y,visual_range, old_set);

    //获得新坐标的可视列表new_set
    std::vector<YY_ENTITYID> new_set;
    get(x, y, visual_range, new_set);

    //delete self
    std::vector<YY_ENTITYID>::iterator itor=find(new_set.begin(), new_set.end(), obj);
    if(itor != new_set.end())
        new_set.erase(itor);

    std::vector<YY_ENTITYID> move_set;
    //同时在old_set和new_set内，则算移动
    // move_set = old_set mix new_set
    for (int i=0; i<(int)old_set.size(); i++)
    {
        YY_ENTITYID tmp_id=old_set[i];
        if (find(new_set.begin(), new_set.end(), tmp_id) != new_set.end())
        {
            m_onmove(tmp_id, obj, x, y);
            move_set.push_back(tmp_id);
        }
    }

    //在old_set内，但是在move_set内找不到，则算离开
    // leave_set = old_set sub move_set
    for (int i=0; i<(int)old_set.size(); i++)
    {
        YY_ENTITYID tmp_id=old_set[i];
        if (find(move_set.begin(), move_set.end(), tmp_id) == move_set.end())
        {
            m_onleave(tmp_id, obj, aoi_scene);
        }
    }

    //在new_set内，但是在move_set内找不到，则算进入
    // enter_set = new_set sub move_set
    for (int i=0; i<(int)new_set.size(); i++)
    {
        YY_ENTITYID tmp_id=new_set[i];
        if (find(move_set.begin(), move_set.end(), tmp_id) == move_set.end())
        {
            m_onenter(tmp_id, obj, aoi_scene);
        }
    }
}

//获得可视列表
void aoi::get(float x, float y, int distance, std::vector<YY_ENTITYID>& v)
{

    //根据坐标获得所在网格
    int row=(int)y/m_distance;
    int column=(int)x/m_distance;

    //获得9个网格
    int index=row*m_column + column;
    append(x, y, distance, index, v);

    if(column<m_column)
    {
        index=row*m_column+column+1;
        append(x, y, distance, index, v);
    }

    if(column>=1)
    {
        index=row*m_column+column-1;
        append(x, y, distance, index, v);
    }

    if(row>=1)
    {
        index=(row-1)*m_column + column;
        append(x, y, distance, index, v);

        if(column<m_column)
        {
            index=(row-1)*m_column+column+1;
            append(x, y, distance, index, v);
        }

        if(column>=1)
        {
            index=(row-1)*m_column+column-1;
            append(x, y, distance, index, v);
        }
    }

    if(row<m_row)
    {
        index=(row+1)*m_column + column;
        append(x, y, distance, index, v);

        if(column<m_column)
        {
            index=(row+1)*m_column+column+1;
            append(x, y, distance, index, v);
        }

        if(column>=1)
        {
            index=(row+1)*m_column+column-1;
            append(x, y, distance, index, v);
        }
    }
}

//把指定网格内的实体集合追加到集合内
void aoi::append(float x, float y, int distance, int index, std::vector<YY_ENTITYID>& v)
{
    if(index >= m_row*m_column)
        return;

    mapblock* mb=&m_blocks[index];
    std::map<YY_ENTITYID, aoi_obj>::iterator itor = mb->obj_set.begin();
    for(; itor != mb->obj_set.end(); itor++)
    {
        const YY_ENTITYID& tmp_id = itor->first;
        const aoi_obj ao = itor->second;

        int tmp_x = (int)ao.x;
        int tmp_y = (int)ao.y;

        //计算两点间距离，判断是否在可视范围内
        //可能为负数，平方为正，所以不要紧
        int diff_x = (int)x - tmp_x;
        int diff_y = (int)y - tmp_y;
        double diff = sqrt ((double)(diff_x*diff_x + diff_y*diff_y));

        if(diff <= distance)
            v.push_back(tmp_id);
    }
}

mapblock* aoi::get_mapblock(float x, float y)
{
    //根据坐标获得所在网格
    int column=(int)x/m_distance;
    int row=(int)y/m_distance;
    int index=row*m_column + column;
    if(index >= m_row*m_column || index<0)
        return NULL;

    mapblock* mb=&m_blocks[index];
    return mb;
}

NS_YY_END