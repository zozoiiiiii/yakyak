/************************************************************************/
/* 
@author:    junliang
@brief:     
1. 利用实参来模板推导，c++ auto也是类似实现
2. 用纯虚函数基类作为对外统一接口，子类用模板实现
(模板是编译时编译器生成代码， 虚函数是运行时调用)
http://www.artima.com/cppsource/type_erasure.html

@time:      2017-5-25
*/
/************************************************************************/
#pragma once
#include <assert.h>
#include <string>

struct MetaField
{
    virtual ~MetaField(){}
    virtual void Set(void* obj, const void* value) const = 0;//{assert(false);}
    virtual void* Get(void* obj) const = 0;// {assert(false); return NULL;}

    std::string name;
    std::string desc;
    int var_type;
    std::string type_name;  // class name
};

template<class ClassType, class FieldType>
class MetaMemberField : public MetaField
{
public:
    MetaMemberField(FieldType ClassType::* field);
    virtual void Set(void* obj, const void* value) const;
    virtual void* Get(void* obj) const;

private:
    typedef  FieldType ClassType::*fptr;
    fptr f;
};

template<class ClassType, class FieldType>
MetaMemberField<ClassType, FieldType>::MetaMemberField(FieldType ClassType::* field)
{
    this->f = field;
    var_type = VarHelper<FieldType>::GetType();
    type_name = VarHelper<FieldType>::GetTypeName();
}


template<class ClassType, class FieldType>
void MetaMemberField<ClassType, FieldType>::Set(void* obj, const void* value) const
{
    ((ClassType*)obj)->*f = *(const FieldType*)value;
}

//     virtual void Get(void* obj, void* value) const{
//         *(FieldType*)value = (__C*)obj->*f;
//     }

template<class ClassType, class FieldType>
void* MetaMemberField<ClassType, FieldType>::Get(void* obj) const
{
    return &(((ClassType*)obj)->*f);
}

// 通过实参(类成员变量指针)来推导出类成员变量类型
template<class ClassType, class FieldType>
MetaField* MakeField(FieldType ClassType::* field)
{
    // 把推导出来的信息保存到实例化的类里
    return new MetaMemberField<ClassType, FieldType>(field);
}

