/************************************************************************/
/* 
@author:    junliang
@brief:     coroutine in c++

项目属性：编辑并继续/ZI 会导致__LINE__不是常量的编译错误，改成程序数据库/Zi就可以。

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