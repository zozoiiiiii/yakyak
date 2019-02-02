/************************************************************************/
/* 
@author:    junliang
@brief:     
1. like c++ auto, use parameter to generate from template
2. virtual function used in runtime, template function used in compile time
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
    int var_type;		// YVarType
    std::string type_name;  // "bool", or class name
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

// get the member type from the instance member
template<class ClassType, class FieldType>
MetaField* MakeField(FieldType ClassType::* field)
{
	// save the class type information
    return new MetaMemberField<ClassType, FieldType>(field);
}

