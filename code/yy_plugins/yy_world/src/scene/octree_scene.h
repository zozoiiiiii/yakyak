/************************************************************************/
/* 
@author:    junliang
@brief:     八叉树场景管理
@time:      2017-8-23
*/
/************************************************************************/
#pragma once


#include "math/inc/yy_aabb.h"
#include "math/inc/yy_ray.h"
#include "yy_world/inc/i_game_obj.h"

using namespace YY;

// 八叉树节点
struct OctreeNode
{
    AABB aabb;
    std::vector<IGameObj*> m_drawlist;
    OctreeNode * m_child[8];


    OctreeNode()
    {
        for(int i =0;i<8;i++)
        {
            m_child[i] = NULL;
        }
    }

};

class OctreeScene : public YY::BaseObject
{
    YY_BEGIN(OctreeScene, "BaseObject");
    YY_END
public:
    OctreeScene();
    void init(AABB range);
    void addObj(IGameObj * obj);

    // ? no remove from m_objList
    void removeObj(IGameObj * obj);
    void updateObj(IGameObj * obj);
    IGameObj* hitByRay(const Ray &ray, Vec3f &hitPoint);

    // 根据摄像机裁剪 获得可以显示的所有对象
    void cullingByCamera(IGameObj * camera);
    void getRange(std::vector<IGameObj *> * list,AABB aabb);
    int getAmount();
    std::vector<YY_OBJECTID>& getVisibleList();
    std::vector<YY_OBJECTID>& getAllObjList();

	bool isInOctree(IGameObj * obj);
    IGameObj* FindObjByName(const std::string& name);

private:
    int getAmount_R(OctreeNode * node);
    void cullingByCamera_R(OctreeNode * node,IGameObj * camera);
    void setIndrawable_R(OctreeNode * node);
    void setDrawable_R(OctreeNode * node);

    // 获得该节点下所有和包围盒相交的可视对象
    void getRange_R(OctreeNode * node, std::vector<IGameObj *> * list, AABB &aabb);

    // 遍历该节点的所有子节点，找到最匹配该包围盒的一个节点，记录到该节点的对象列表中
    bool addObj_R(OctreeNode * node,IGameObj * obj);
    bool removeObj_R(OctreeNode * node,IGameObj * obj);

    // 遍历该节点的所有子节点，找到一个最匹配的包围盒的相交点
    IGameObj* hitByRay_R(OctreeNode * node,const Ray &ray, Vec3f & hitPoint);

    // 分配八叉树节点内存
    void subDivide(OctreeNode * node,int level);

private:
    OctreeNode * m_root;    // 三维空间上八等分切割的八叉树数据结构
    std::vector<YY_OBJECTID> m_visibleList;
	std::vector<YY_OBJECTID> m_objList;
};

