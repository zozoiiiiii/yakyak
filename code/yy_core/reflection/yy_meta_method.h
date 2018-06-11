/************************************************************************/
/* 
@author:    junliang
@brief:     根据函数指针，获得对应的参数，返回值
@time:      8/16/2016
*/
/************************************************************************/
#pragma once


#include <vector>
#include "yy_var_helper.h"


struct MetaMethod
{
    virtual ~MetaMethod(){}
    virtual void Invoke(void* result, void* obj, void* parameters[]) = 0;

    struct Param
    {
        int var_type;
        VarFlag flag;
        bool isOut;
        Param():var_type(YVT_UNKNOWN),isOut(false){}
    };

    std::string name;
    std::string desc;
    bool is_static;
    Param returnParam;
    std::vector<Param> params;
};

template<class __RT, class __C>
class Method0 : public MetaMethod
{ 
public:
    typedef __RT (__C::*fptr)();
    fptr f;

    Method0(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<__RT>::GetType();
        
    }

    virtual void Invoke(void* result, void* obj, void* parameters[])
    {
        *(__RT*)result = (((__C*)obj)->*f)();
    }
};

template<class __RT, class __C, class __P1>
class Method1 : public MetaMethod
{ 
public:
    typedef __RT (__C::*fptr)(__P1);
    fptr f;

    Method1(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<__RT>::GetType();
        Param param0;
        param0.var_type = VarHelper<__P1>::GetType();
        param0.isOut = VarHelper<__P1>::IsOut();
        param0.flag = VarHelper<__P1>::GetFlag();
        params.push_back(param0);
    }

    virtual void Invoke(void* result, void* obj, void* parameters[]){
        *(__RT*)result = (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]));
    }
};

template<class __RT, class __C, class __P1, class __P2>
class Method2 : public MetaMethod
{ 
public:
    typedef __RT (__C::*fptr)(__P1, __P2);
    fptr f;

    Method2(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<__RT>::GetType();
        Param param0;
        param0.var_type = VarHelper<__P1>::GetType();
        param0.isOut = VarHelper<__P1>::IsOut();
        param0.flag = VarHelper<__P1>::GetFlag();
        params.push_back(param0);

        Param param1;
        param1.var_type = VarHelper<__P2>::GetType();
        param1.isOut = VarHelper<__P2>::IsOut();
        param1.flag = VarHelper<__P2>::GetFlag();
        params.push_back(param1);
    }

    virtual void Invoke(void* result, void* obj, void* parameters[]){
        *(__RT*)result = (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]),
            VarHelper<__P2>::Get(parameters[1]));
    }
};


template<class __RT, class __C, class __P1, class __P2, class __P3>
class Method3 : public MetaMethod
{ 
public:
    typedef __RT (__C::*fptr)(__P1, __P2, __P3);
    fptr f;

    Method3(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<__RT>::GetType();
        Param param0;
        param0.var_type = VarHelper<__P1>::GetType();
        param0.isOut = VarHelper<__P1>::IsOut();
        param0.flag = VarHelper<__P1>::GetFlag();
        params.push_back(param0);

        Param param1;
        param1.var_type = VarHelper<__P2>::GetType();
        param1.isOut = VarHelper<__P2>::IsOut();
        param1.flag = VarHelper<__P2>::GetFlag();
        params.push_back(param1);


        Param param2;
        param2.var_type = VarHelper<__P3>::GetType();
        param2.isOut = VarHelper<__P3>::IsOut();
        param2.flag = VarHelper<__P3>::GetFlag();
        params.push_back(param2);
    }

    virtual void Invoke(void* result, void* obj, void* parameters[]){
        *(__RT*)result = (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]),
            VarHelper<__P2>::Get(parameters[1]),
            VarHelper<__P3>::Get(parameters[2]));
    }
};


template<class __RT, class __C, class __P1, class __P2, class __P3, class __P4>
class Method4 : public MetaMethod
{ 
public:
    typedef __RT (__C::*fptr)(__P1, __P2, __P3, __P4);
    fptr f;

    Method4(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<__RT>::GetType();
        Param param0;
        param0.var_type = VarHelper<__P1>::GetType();
        param0.isOut = VarHelper<__P1>::IsOut();
        param0.flag = VarHelper<__P1>::GetFlag();
        params.push_back(param0);

        Param param1;
        param1.var_type = VarHelper<__P2>::GetType();
        param1.isOut = VarHelper<__P2>::IsOut();
        param1.flag = VarHelper<__P2>::GetFlag();
        params.push_back(param1);


        Param param2;
        param2.var_type = VarHelper<__P3>::GetType();
        param2.isOut = VarHelper<__P3>::IsOut();
        param2.flag = VarHelper<__P3>::GetFlag();
        params.push_back(param2);

        Param param3;
        param3.var_type = VarHelper<__P4>::GetType();
        param3.isOut = VarHelper<__P4>::IsOut();
        param3.flag = VarHelper<__P4>::GetFlag();
        params.push_back(param3);
    }

    virtual void Invoke(void* result, void* obj, void* parameters[]){
        *(__RT*)result = (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]),
            VarHelper<__P2>::Get(parameters[1]),
            VarHelper<__P3>::Get(parameters[2]),
            VarHelper<__P4>::Get(parameters[3]));
    }
};


template<class __RT, class __C>
inline MetaMethod* MakeMethod(__RT (__C::*f)()){
    return new Method0<__RT, __C>(f);
}

template<class __RT, class __C, class __P1>
inline MetaMethod* MakeMethod(__RT (__C::*f)(__P1)){
    return new Method1<__RT, __C, __P1>(f);
}

template<class __RT, class __C, class __P1, class __P2>
inline MetaMethod* MakeMethod(__RT (__C::*f)(__P1, __P2)){
    return new Method2<__RT, __C, __P1, __P2>(f);
}


template<class __RT, class __C, class __P1, class __P2, class __P3>
inline MetaMethod* MakeMethod(__RT (__C::*f)(__P1, __P2, __P3)){
    return new Method3<__RT, __C, __P1, __P2, __P3>(f);
}

template<class __RT, class __C, class __P1, class __P2, class __P3, class __P4>
inline MetaMethod* MakeMethod(__RT (__C::*f)(__P1, __P2, __P3, __P4)){
    return new Method4<__RT, __C, __P1, __P2, __P3, __P4>(f);
}

