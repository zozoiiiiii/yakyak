#include "input_mgr.h"

void InputMgr::AddMsgProc(YY::Entity* pEntity, uint32 msg_id)
{
    throw_assert(NULL!=pEntity, "null check.");

    std::vector<YY::Entity*>& entities = m_listener[msg_id];

    // makesure not exist.
    int nSize = entities.size();
    for(int i=0; i<nSize; i++)
    {
        YY::Entity* pTmpEntity = entities[i];
        if(pTmpEntity->GetID() == pEntity->GetID())
        {
            return;
        }
    }

    // add
    entities.push_back(pEntity);
}

void InputMgr::RemoveMsgProc(YY::Entity* pEntity, uint32 msg_id)
{
    throw_assert(NULL!=pEntity, "null check.");

    std::map<int, std::vector<YY::Entity*> >::iterator itor = m_listener.find(msg_id);
    if(itor == m_listener.end())
        return;

    std::vector<YY::Entity*>& entities = itor->second;
    int nSize = entities.size();
    for(int i=0; i<nSize; i++)
    {
        YY::Entity* pTmpEntity = entities[i];
        if(pTmpEntity->GetID() == pEntity->GetID())
        {
            entities.erase(entities.begin() + i);
            return;
        }
    }
}

bool InputMgr::Invoke(uint32 msg_id, uint32 param1, uint32 param2)
{
    std::map<int, std::vector<YY::Entity*> >::iterator itor = m_listener.find(msg_id);
    if(itor == m_listener.end())
        return false;   // use default msg proc

    std::vector<YY::Entity*>& entities = itor->second;
    int nSize = entities.size();
    for(int i=0; i<nSize; i++)
    {
        YY::Entity* pTmpEntity = entities[i];
        bool ret = pTmpEntity->OnMsg(msg_id, param1, param2);
        if(!ret) // use default msg proc
            return false;
    }

    // do not use default msg proc
    return true;
}
