/************************************************************************/
/* 
@author:    junliang
@brief:     �˲�����������
@time:      2017-8-23
*/
/************************************************************************/
#pragma once


#include "math/inc/yy_aabb.h"
#include "math/inc/yy_ray.h"
#include "yy_world/inc/i_game_obj.h"

using namespace YY;

// �˲����ڵ�
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

    // ����������ü� ��ÿ�����ʾ�����ж���
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

    // ��øýڵ������кͰ�Χ���ཻ�Ŀ��Ӷ���
    void getRange_R(OctreeNode * node, std::vector<IGameObj *> * list, AABB &aabb);

    // �����ýڵ�������ӽڵ㣬�ҵ���ƥ��ð�Χ�е�һ���ڵ㣬��¼���ýڵ�Ķ����б���
    bool addObj_R(OctreeNode * node,IGameObj * obj);
    bool removeObj_R(OctreeNode * node,IGameObj * obj);

    // �����ýڵ�������ӽڵ㣬�ҵ�һ����ƥ��İ�Χ�е��ཻ��
    IGameObj* hitByRay_R(OctreeNode * node,const Ray &ray, Vec3f & hitPoint);

    // ����˲����ڵ��ڴ�
    void subDivide(OctreeNode * node,int level);

private:
    OctreeNode * m_root;    // ��ά�ռ��ϰ˵ȷ��и�İ˲������ݽṹ
    std::vector<YY_OBJECTID> m_visibleList;
	std::vector<YY_OBJECTID> m_objList;
};

