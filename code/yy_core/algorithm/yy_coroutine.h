/************************************************************************/
/* 
@author:    junliang
@brief:     coroutine

//http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html

@sample:    algorithm->coroutine test
@compile error:  /ZI -> /Zi, because of case __LINE__

@time:      2017-6-26
*/
/************************************************************************/
#pragma once


#define scrBegin         static int scrLine = 0; switch(scrLine) { case 0:;
#define scrFinish(z)     } scrLine = 0;return (z)
#define scrReturn(z)     \
    do {\
    scrLine=__LINE__;\
    return (z); case __LINE__:;\
    } while (0)
