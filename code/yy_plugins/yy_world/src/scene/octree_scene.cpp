#include "octree_scene.h"
#include <algorithm>
#include <math.h>
#include "yy_world/inc/components/i_comp_camera.h"

#define MAX_DEEP 3


OctreeScene::OctreeScene()
{

}

void OctreeScene::init(AABB range)
{
    m_root = new OctreeNode();
    m_root->aabb = range;
    subDivide(m_root,1);
}

void OctreeScene::subDivide(OctreeNode *node, int level)
{
    if(level> MAX_DEEP)
        return;

    // 当前包围盒八等分
    std::vector<AABB> subAABBList = node->aabb.split8();
    for(int i =0;i<8;i++)
    {
        node->m_child[i] = new OctreeNode();
        node->m_child[i]->aabb = subAABBList[i];
        subDivide(node->m_child[i],level + 1 );
    }
}

std::vector<YY_OBJECTID> &OctreeScene::getVisibleList()
{
    return m_visibleList;
}

std::vector<YY_OBJECTID> &OctreeScene::getAllObjList()
{
    return m_objList;
}

bool OctreeScene::isInOctree(IGameObj * obj)
{
    int nSize = m_objList.size();
    for(int i=0; i<nSize; i++)
    {
        if(m_objList[i] == obj->GetID())
            return true;
    }

    return false;
}



IGameObj* OctreeScene::FindObjByName(const std::string& name)
{
    int nSize = m_objList.size();
    for(int i=0; i<nSize; i++)
    {
        YY_OBJECTID id = m_objList[i];
        IGameObj* pGameObj = (IGameObj*)GetMgr()->Find(id);
        if(pGameObj && pGameObj->GetName() == name)
            return pGameObj;
    }

    return NULL;
}

void OctreeScene::addObj(IGameObj *obj)
{
    // 从根节点开始查找合适的八叉树节点
    bool result = addObj_R(m_root,obj);
    if(result)
    {
		m_objList.push_back(obj->GetID());
    }
}

bool OctreeScene::addObj_R(OctreeNode *node, IGameObj *obj)
{
    // 对象包围盒是否在 当前节点的空间 内
    if(node->aabb.isCanCotain(obj->GetAABB()))
    {
        // 没有子节点的话，挂在自己身上
        if(NULL == node->m_child[0])//terminal node
        {
            node->m_drawlist.push_back(obj);
            return true;
        }
        // 有子节点的话，递归找到最后一个可以和包围盒相交的节点，即找到和包围盒最匹配的节点
        else
        {
            for(int i =0;i<8;i++)
            {
                if(addObj_R(node->m_child[i],obj))
                {
                    return true;
                }
                // the children can containt that ,so we only can put it in parent.
                node->m_drawlist.push_back(obj);
                return true;
            }
        }
    }
    return false;
}

void OctreeScene::removeObj(IGameObj *obj)
{
    removeObj_R(m_root,obj);
}

bool OctreeScene::removeObj_R(OctreeNode *node, IGameObj *obj)
{
    std::vector<IGameObj*>::iterator result = std::find(node->m_drawlist.begin(),node->m_drawlist.end(),obj);
    if(result != node->m_drawlist.end())
    {
        node->m_drawlist.erase(result);
        return true;
    }
    else
    {
        if(!node->m_child[0])
            return false;//terminal node.

        for(int i =0;i<8;i++)
        {
            if(removeObj_R(node->m_child[i],obj))
            {
                return true;
            }
        }
        return false;
    }
}

IGameObj* OctreeScene::hitByRay_R(OctreeNode *node, const Ray &ray, Vec3f &hitPoint)
{
	// 当前节点被射线选中
    Vec3f result;
    if(!ray.intersectAABB(node->aabb,NULL,result))
        return NULL;

    //check self, 判断当前节点里的所有可视对象
    for(int i =0;i<node->m_drawlist.size();i++)
    {
        IGameObj * obj = node->m_drawlist[i];
        if(ray.intersectAABB(obj->GetAABB_World(),NULL,hitPoint))
        {
            return obj;
        }
    }

    //check sub
    if(!node->m_child[0])
        return false; // is terminal

    for(int i = 0;i<8;i++)
    {
        IGameObj* pObj = hitByRay_R(node->m_child[i],ray,hitPoint);
        if(pObj)
        {
            return pObj;
        }
    }

    return NULL;
}

void OctreeScene::updateObj(IGameObj *obj)
{
    removeObj(obj);
    addObj(obj);
}

IGameObj* OctreeScene::hitByRay(const Ray &ray, Vec3f &hitPoint)
{
    return hitByRay_R(m_root,ray,hitPoint);
}

void OctreeScene::cullingByCamera(IGameObj *camera)
{
	if (nullptr == camera)
		return;

    //clear visible List;
    m_visibleList.clear ();

    // 设置场景中所有对象可视
    //reset the state
    setDrawable_R(m_root);

    // 通过计算，设置哪些可视，哪些不可视
    //camera culling
    cullingByCamera_R(m_root,camera);
}

void OctreeScene::getRange(std::vector<IGameObj *> *list, AABB aabb)
{
    getRange_R(m_root,list,aabb);
}

int OctreeScene::getAmount()
{
    return getAmount_R(m_root);
}

int OctreeScene::getAmount_R(OctreeNode *node)
{
    int the_size = node->m_drawlist.size();
    if(!node->m_child[0]) return the_size;
    for(int i =0;i<8;i++)
    {
        the_size += getAmount_R(node->m_child[i]);
    }
    return the_size;
}

static IGameObj * currentCamera = NULL;
static int compare(const void * a, const void * b)
{
    OctreeNode * nodeA = (OctreeNode *)a;
    OctreeNode * nodeB = (OctreeNode *)b;
    Vec3f vA = nodeA->aabb.centre() - currentCamera->GetTransform()->GetPos();
    Vec3f vB = nodeB->aabb.centre() - currentCamera->GetTransform()->GetPos();
    float distA  = fabs(vA.x) + fabs(vA.y) + fabs(vA.z);
    float distB  = fabs(vB.x) + fabs(vB.y) + fabs(vB.z);
    if(distA > distB)
    {
        return 1;
    }
    else if(fabs(distA  - distB) < 0.00001)
    {
        return 0;
    }else
    {
        return -1;
    }
}

void OctreeScene::cullingByCamera_R(OctreeNode *node, IGameObj *camera)
{
    IComp_Camera* pCameraComponent = (IComp_Camera*)camera->FindComponent("Comp_Camera");
    if(pCameraComponent->isOutOfFrustum(node->aabb))
    {
        //set In drawable
        setIndrawable_R(node);
    }
    else
    {
        for(int i=0;i<node->m_drawlist.size();i++)
        {
            IGameObj * obj = node->m_drawlist[i];
            if(!pCameraComponent->isOutOfFrustum(obj->GetAABB()))
            {
                obj->SetVisible(true);
                m_visibleList.push_back (obj->GetID());
            }
            else
            {
                obj->SetVisible(false);
            }
        }

        if(!node->m_child[0])
            return;//terminal node return directly

        currentCamera = camera;

        // 按照距离摄像机距离 来排序
        qsort(node->m_child,8,sizeof(OctreeNode *),compare);
        for(int i=0;i<8;i++)
        {
            cullingByCamera_R(node->m_child[i],camera);
        }
    }
}

void OctreeScene::setIndrawable_R(OctreeNode *node)
{
    for(int i=0;i<node->m_drawlist.size();i++)
    {
        IGameObj * obj = node->m_drawlist[i];
        obj->SetVisible(false);
    }
    if(!node->m_child[0])
        return;//terminal node return directly

    for(int i=0;i<8;i++)
    {
        setIndrawable_R(node->m_child[i]);
    }
}

void OctreeScene::setDrawable_R(OctreeNode *node)
{
    for(int i=0;i<node->m_drawlist.size();i++)
    {
        IGameObj * obj = node->m_drawlist[i];
        obj->SetVisible(true);
    }

    if(!node->m_child[0])
        return;//terminal node return directly

    for(int i=0;i<8;i++)
    {
        setDrawable_R(node->m_child[i]);
    }
}

void OctreeScene::getRange_R(OctreeNode *node, std::vector<IGameObj *> *list, AABB &aabb)
{
    Vec3f noNeedVar;
    if(node->aabb.isIntersect(aabb,noNeedVar))
    {
        //put self
        for(int i =0;i<node->m_drawlist.size();i++)
        {
            // 如果包围盒相交
            IGameObj* obj = node->m_drawlist[i];
            if(obj->GetAABB().isIntersect(aabb,noNeedVar))
            {
                list->push_back(node->m_drawlist[i]);
            }
        }
        //check sub
        if(!node->m_child[0])
            return;

        for(int i =0;i<8;i++)
        {
            getRange_R(node->m_child[i],list,aabb);
        }
    }
}



