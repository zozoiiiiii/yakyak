/************************************************************************/
/*
@author:  junliang
@brief:   macros
@time:    20130401
*/
/************************************************************************/
#ifndef _YY_MACROS_H_
#define _YY_MACROS_H_

// base type
typedef signed __int8                    sint8;
typedef unsigned __int8                  uint8;
typedef signed __int16                   sint16;
typedef unsigned __int16                 uint16;
typedef signed __int32                   sint32;
typedef unsigned __int32                 uint32;
typedef signed __int64                   sint64;
typedef unsigned __int64                 uint64;

// namespace
#ifndef NS_YY_BEGIN
#define NS_YY_BEGIN                    namespace YY {
#define NS_YY_END                    }
#define USING_NS_YY                    using namespace YY;
#endif

//#pragma warning(disable:4996)
#endif