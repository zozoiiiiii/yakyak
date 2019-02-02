/************************************************************************/
/* 
@author:  junliang
@brief:   resource file
@time:    2017/10/21
*/
/************************************************************************/
#pragma once


#include "yy_core.h"
#include "i_res_mgr.h"

class IRender;
class IRes : public YY::BaseObject
{
	YY_VIRTUAL_BEGIN(IRes, BaseObject);
	YY_END
public:
    IResMgr* GetResMgr(){return m_pResMgr;}
    IRender* GetRender(){return m_pResMgr->GetRender();}
private:
    void SetResMgr(IResMgr* pResMgr){m_pResMgr = pResMgr;}
private:
    friend class ResMgr;
    IResMgr* m_pResMgr;
};
