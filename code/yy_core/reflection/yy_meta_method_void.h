/************************************************************************/
/* 
@author:    junliang
@brief:     根据函数指针，获得对应的参数，返回值
@time:      8/16/2016
*/
/************************************************************************/
#pragma once

#include "yy_meta_method.h"


template<class __C>
class Method0_Void : public MetaMethod
{ 
public:
    typedef void (__C::*fptr)();
    fptr f;

    Method0_Void(fptr f)
    {
        this->f = f;
    }

    virtual void Invoke(void*, void* obj, void* parameters[])
    {
        (((__C*)obj)->*f)();
    }
};

template<class __C, class __P1>
class Method1_Void : public MetaMethod
{ 
public:
    typedef void (__C::*fptr)(__P1);
    fptr f;

    Method1_Void(fptr f)
    {
        this->f = f;
        Param param0;
        param0.var_type = VarHelper<__P1>::GetType();
        param0.isOut = VarHelper<__P1>::IsOut();
        param0.flag = VarHelper<__P1>::GetFlag();
        params.push_back(param0);
    }

    virtual void Invoke(void*, void* obj, void* parameters[]){
        (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]));
    }
};

template<class __C, class __P1, class __P2>
class Method2_Void : public MetaMethod
{ 
public:
    typedef void (__C::*fptr)(__P1, __P2);
    fptr f;

    Method2_Void(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<void>::GetType();
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

    virtual void Invoke(void*, void* obj, void* parameters[]){
        (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]),
            VarHelper<__P2>::Get(parameters[1]));
    }
};


template<class __C, class __P1, class __P2, class __P3>
class Method3_Void : public MetaMethod
{ 
public:
    typedef void (__C::*fptr)(__P1, __P2, __P3);
    fptr f;

    Method3_Void(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<void>::GetType();
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

    virtual void Invoke(void*, void* obj, void* parameters[]){
        (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]),
            VarHelper<__P2>::Get(parameters[1]),
            VarHelper<__P3>::Get(parameters[2]));
    }
};


template<class __C, class __P1, class __P2, class __P3, class __P4>
class Method4_Void : public MetaMethod
{ 
public:
    typedef void (__C::*fptr)(__P1, __P2, __P3, __P4);
    fptr f;

    Method4_Void(fptr f)
    {
        this->f = f;
        returnParam.var_type = VarHelper<void>::GetType();
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

    virtual void Invoke(void*, void* obj, void* parameters[]){
        (((__C*)obj)->*f)(VarHelper<__P1>::Get(parameters[0]),
            VarHelper<__P2>::Get(parameters[1]),
            VarHelper<__P3>::Get(parameters[2]),
            VarHelper<__P4>::Get(parameters[3]));
    }
};


template<class __C>
inline MetaMethod* MakeMethod(void (__C::*f)()){
    return new Method0_Void<__C>(f);
}

template<class __C, class __P1>
inline MetaMethod* MakeMethod(void (__C::*f)(__P1)){
    return new Method1_Void<__C, __P1>(f);
}

template<class __C, class __P1, class __P2>
inline MetaMethod* MakeMethod(void (__C::*f)(__P1, __P2)){
    return new Method2_Void<__C, __P1, __P2>(f);
}


template<class __C, class __P1, class __P2, class __P3>
inline MetaMethod* MakeMethod(void (__C::*f)(__P1, __P2, __P3)){
    return new Method3_Void< __C, __P1, __P2, __P3>(f);
}

template<class __C, class __P1, class __P2, class __P3, class __P4>
inline MetaMethod* MakeMethod(void (__C::*f)(__P1, __P2, __P3, __P4)){
    return new Method4_Void< __C, __P1, __P2, __P3, __P4>(f);
}

