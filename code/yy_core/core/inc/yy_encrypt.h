/************************************************************************/
/*
@author:  junliang
@brief:   加密解密

一次异或明文变密文；二次加密就是密文变明文, 天龙八部是异或加密方式

@time:    20130609
@sample:

    char str[]="hello, world中文fwoqj990";
    int len=strlen(str);
    ENCRYPT(str, strlen(str), ENCRYPT_KEY);

    len=strlen(str);
    ENCRYPT(str, strlen(str), ENCRYPT_KEY);

*/
/************************************************************************/
#include "reflection/yy_type.h"

//↓这样在可执行文件中将是汉字显示，上线前要改成随机KEY↓KEY不得小于10个字节
#define ENCRYPT_KEY "服务器端与客户端的封包密钥"
//↑这样在可执行文件中将是汉字显示，上线前要改成随机KEY↑KEY不得小于10个字节


#ifndef ENCRYPT
#define ENCRYPT(x,xlen,KEY)    if( (x)!=NULL ) \
        { \
        \
        char* t_pBuffer = (x); \
        char* pKey = {KEY}; \
        int KeyLen = (int)strlen(pKey); \
        for (int i = 0; i < (xlen); i++) \
            { \
            *t_pBuffer ^= pKey[(i)%KeyLen]; \
            t_pBuffer++; \
            } \
        }
#endif