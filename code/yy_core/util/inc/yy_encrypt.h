/************************************************************************/
/*
@author:  junliang
@brief:   ���ܽ���

һ��������ı����ģ����μ��ܾ������ı�����, �����˲��������ܷ�ʽ

@time:    20130609
@sample:

    char str[]="hello, world����fwoqj990";
    int len=strlen(str);
    ENCRYPT(str, strlen(str), ENCRYPT_KEY);

    len=strlen(str);
    ENCRYPT(str, strlen(str), ENCRYPT_KEY);

*/
/************************************************************************/
#include "reflection/yy_type.h"

//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�
#define ENCRYPT_KEY "����������ͻ��˵ķ����Կ"
//�������ڿ�ִ���ļ��н��Ǻ�����ʾ������ǰҪ�ĳ����KEY��KEY����С��10���ֽ�


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