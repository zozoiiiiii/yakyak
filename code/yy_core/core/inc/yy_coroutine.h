/************************************************************************/
/* 
@author:    junliang
@brief:     coroutine in c++

��Ŀ���ԣ��༭������/ZI �ᵼ��__LINE__���ǳ����ı�����󣬸ĳɳ������ݿ�/Zi�Ϳ��ԡ�

https://www.chiark.greenend.org.uk/~sgtatham/coroutine.h
@time:      2017-6-22
*/
/************************************************************************/
#pragma once

#include <stdlib.h>

/*
 * `scr' macros for static coroutines.
 */

#define scrBegin         static int scrLine = 0; switch(scrLine) { case 0:;
#define scrFinish(z)     } scrLine = 0;return (z)
#define scrReturn(z)     \
        do {\
            scrLine=__LINE__;\
            return (z); case __LINE__:;\
        } while (0)