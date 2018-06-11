/************************************************************************/
/* 
@author:    junliang
@brief:     ö��tostring
@time:      10/20/2016
*/
/************************************************************************/
#pragma once


#define ENUM_VALUES    \
    ENUM_VALUE(CCOMMAND_CREATE_OBJ, "��������")\
    ENUM_VALUE(CCOMMAND_REMOVE_OBJ, "ɾ������")\
    ENUM_VALUE(CCOMMAND_OBJ_DEFINE_MAX,"����״̬��ض������ֵ") \
    /*aa*/\
    ENUM_VALUE(CCOMMAND_CREATE_ROLE_SUCCESS,                            "���Ǵ����Լ��Ľ�ɫ") \
    ENUM_VALUE(CCOMMAND_PLAT_DEFINE_MAX,                                    "����ƽ̨֧����ض������ֵ") 


enum CommondType
{
    MIN=0,
#define ENUM_VALUE(NAME, DESC) NAME,
    ENUM_VALUES
#undef ENUM_VALUE
    MAX,
};

inline void ToStr(int cType, std::string& name, std::string& desc)
{
    switch (cType)
    {
#define ENUM_VALUE(NAME, DESC) case NAME: {name=#NAME;desc=DESC;}break;
        ENUM_VALUES
#undef ENUM_VALUE
    default:
        //assert(false);
        break;
    }
}


//int main() {
//    std::string name;
//    std::string desc;
//    ToStr(1, name, desc);
//    ToStr(CCOMMAND_REMOVE_OBJ, name, desc);
//    return 0;
//}
