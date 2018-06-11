#include"mmq/inc/yy_mmq.h"
#include "net/impl_win/win_iocp.h"
#include "net/mmq_select.h"
#include "sharemem/mmq_sharemem.h"
#include "core/inc/yy_exception.h"

NS_YY_BEGIN
MMQ* CreateMQ(int nType, CB_OnCon con, CB_OnDiscon discon, CB_OnRead read)
{
    MMQ* pMQ = NULL;

    switch (nType)
    {
    case MT_SHM:
        {
            pMQ = new MMQ_ShareMem(con, discon, read);
        }
        break;
    case MT_TCP_SELECT:
        {
            pMQ = new MMQ_Select(con, discon, read);
        }
        break;
    case MT_TCP_SUPER:
        {
            pMQ = new Win_IOCP(con, discon, read);
        }
        break;
    default:
        throw_assert(false, "not support mmq type:" << nType);
        break;
    }

    return pMQ;
}

NS_YY_END