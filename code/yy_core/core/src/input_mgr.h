/************************************************************************/
/* 
@author:    junliang
@brief:     input
@time:      2017-6-30
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include "core/inc/yy_object.h"
#include "core/inc/yy_ObjectMgr.h"

class InputMgr
{
public:
    void AddMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
	void RemoveMsgProc(YY::BaseObject* pBaseObject, uint32 msg_id);
    bool Invoke(uint32 msg_id, uint32 param1, uint32 param2);

private:
    std::map<int, std::vector<YY::BaseObject*> > m_listener;
};