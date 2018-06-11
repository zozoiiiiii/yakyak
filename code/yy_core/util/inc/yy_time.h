/************************************************************************/
/*
@author:  junliang
@brief:   time
http://www.cplusplus.com/reference/ctime

@time:    20130510
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include <string>
NS_YY_BEGIN




//get micro seconds from epoch
sint64 TimeStamp();

//string time format: 2014-04-08 11:04:48.21348
std::string StrTimeStamp();

NS_YY_END