#include "game_obj.h"
#include "yy_world/inc/i_world.h"
#include "yy_world/inc/i_scene.h"

GameObj::GameObj()
{
    m_pParent = NULL;
    m_receive_shadow = false;
    m_cast_shadow = false;
}


void GameObj::OnCreate(const VariantMap& args)
{}

IRender* GameObj::GetRender()
{
    return GetScene()->GetRender();
}
// 
// IComponent* GameObj::AddComp(const std::string& name)
// {
//     throw_assert(NULL == FindComp(name), "already exist.");
// 
//     YY::BaseObject* pBaseObject = GetMgr()->Create(name);
//     throw_assert(NULL!=pBaseObject, "null check.");
//     throw_assert(pBaseObject->IsInstanceOf(name), "type check.");
//     IComponent* pComp = (IComponent*)pBaseObject;
//     pComp->SetOwner(this);
//     m_comps.push_back(pComp);
//     return pComp;
// }
// 
// IComponent* GameObj::FindComp(const std::string& name)
// {
//     int nCompSize = (int)m_comps.size();
//     for (int i = 0; i < nCompSize; i++)
//     {
//         IComponent* pComp = m_comps[i];
//         if (pComp->GetMetaClass()->name == name)
//             return pComp;
//     }
// 
//     return NULL;
// }
// 
// IComponent* GameObj::GetComp(const std::string& name)
// {
//     IComponent* pComp = FindComp(name);
//     throw_assert(NULL != pComp, "cannot find " << name);
//     return pComp;
// }



void GameObj::OnUpdate(float sec)
{
    int nSize = m_comps.size();
    for(int i=0; i<nSize; i++)
    {
        IComponent* pIComponent = m_comps[i];
       // pIComponent->OnUpdate(sec);
    }
}

void GameObj::OnAddRender(IBatchGroup* pBatchGroup)
{
    int nSize = m_comps.size();
    for(int i=0; i<nSize; i++)
    {
        IComponent* pIComponent = m_comps[i];
        //pIComponent->OnAddRender(pBatchGroup);
    }
}

void GameObj::OnRender(RenderContext* pCxt)
{
    int nSize = m_comps.size();
    for(int i=0; i<nSize; i++)
    {
        IComponent* pIComponent = m_comps[i];
        //pIComponent->OnRender(pCxt);
    }
}

// 
// void GameObj::SetParent(IGameObj* pObj)
// {
//     m_pParent = pObj;
// }
// 
// IGameObj* GameObj::FindParent()
// {
//     return m_pParent;
// }
// void GameObj::GetChildren(std::vector<IGameObj*>& objs)
// {
//     objs = m_childrens;
// }
// 
// 
// void GameObj::AddChild(IGameObj* pObj)
// {
//     m_childrens.push_back(pObj);
// }


bool GameObj::SerializeTo(YY::VarList& args)
{
    YY::VarList gameobj_args;
//    bool result = YY::BaseObject::SerializeTo(gameobj_args);
  //  if(!result)
        return false;

    int game_obj_cnt = gameobj_args.GetCount();
    args<<game_obj_cnt;
    args.Append(gameobj_args, 0, game_obj_cnt);

    // IComponents
    int nIComponentCnt = m_comps.size();
    args << nIComponentCnt;
    for(int i=0; i<nIComponentCnt; i++)
    {
        IComponent* pComp = m_comps[i];
        YY::VarList comp_args;
//        if(!pComp->SerializeTo(comp_args))
            return false;

        int comp_args_cnt = comp_args.GetCount();
       ///// args<< pComp->GetMetaClass()->name <<comp_args_cnt;
        args.Append(comp_args, 0, comp_args_cnt);
    }

    // childrens
    // not support now

    return true;
}

bool GameObj::ParseFrom(const YY::VarList& args, int& read_index__)
{
    int read_index=0;
    int gameobj_cnt = args.GetInt(read_index);
    read_index+=1;

    YY::VarList gameobj_args;
    gameobj_args.Append(args, read_index, gameobj_cnt);
    read_index += gameobj_cnt;

    //bool result = YY::BaseObject::ParseFrom(gameobj_args, read_index__);
    //if(!result)
        return false;


	this->SetName("parsefrom_something.");
//	m_pScene->AddObj(this);
	SetReceiveShadow(true);

    // IComponents
    int nIComponentCnt = args.GetInt(read_index);
    read_index += 1;
    for(int i=0; i<nIComponentCnt; i++)
    {
        std::string comp_name = args.GetStr(read_index);
        read_index += 1;

       ///// IComponent* pComp = AddComp(comp_name);
        int comp_args_cnt = args.GetInt(read_index);
        read_index+=1;

        YY::VarList comp_args;
        comp_args.Append(args, read_index, comp_args_cnt);
        read_index += comp_args_cnt;

       // if(!pComp->ParseFrom(comp_args, read_index__))
         //   return false;
    }

    // childrens
    // not support now

    return true;
}


YY::AABB GameObj::GetAABB_World()
{
    YY::Mat4f tm;
    GetTransform()->GetTMMatrix(&tm);
    YY::AABB aabb_world = m_aabb;
    aabb_world.transForm(tm);
    return aabb_world;
}

YY::AABB GameObj::GetAABB()
{
    return m_aabb;
}

void GameObj::SetAABB(const YY::AABB &aabb)
{
    m_aabb = aabb;
}