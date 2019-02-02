#include "input_mgr.h"

void InputMgr::AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id)
{
    throw_assert(NULL!=pBaseObject, "null check.");

    std::vector<YY::BaseObject*>& entities = m_listener[msg_id];

    // makesure not exist.
    int nSize = entities.size();
    for(int i=0; i<nSize; i++)
    {
        YY::BaseObject* pTmpBaseObject = entities[i];
        if(pTmpBaseObject->GetID() == pBaseObject->GetID())
        {
            return;
        }
    }

    // add
    entities.push_back(pBaseObject);
}

void InputMgr::RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id)
{
    throw_assert(NULL!=pBaseObject, "null check.");

    std::map<int, std::vector<YY::BaseObject*> >::iterator itor = m_listener.find(msg_id);
    if(itor == m_listener.end())
        return;

    std::vector<YY::BaseObject*>& entities = itor->second;
    int nSize = entities.size();
    for(int i=0; i<nSize; i++)
    {
        YY::BaseObject* pTmpBaseObject = entities[i];
        if(pTmpBaseObject->GetID() == pBaseObject->GetID())
        {
            entities.erase(entities.begin() + i);
            return;
        }
    }
}

bool InputMgr::Invoke(uint32 msg_id, uint32 param1, uint32 param2)
{
    std::map<int, std::vector<YY::BaseObject*> >::iterator itor = m_listener.find(msg_id);
    if(itor == m_listener.end())
        return false;   // use default msg proc

    std::vector<YY::BaseObject*>& entities = itor->second;
    int nSize = entities.size();
    for(int i=0; i<nSize; i++)
    {
        YY::BaseObject* pTmpBaseObject = entities[i];
        bool ret = pTmpBaseObject->OnMsg(msg_id, param1, param2);
        if(!ret) // use default msg proc
            return false;
    }

    // do not use default msg proc
    return true;
}
