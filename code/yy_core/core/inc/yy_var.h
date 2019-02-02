/************************************************************************/
/* 
@author:  junliang
@brief:   deep copy, not c struct
@time:    2017/11/6
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include <string.h>
#include <string>
#include <wchar.h>


NS_YY_BEGIN


class Var
{
public:
    Var();
    Var(bool val);
    Var(int val);
    Var(sint64 val);
    Var(float val);
    Var(double val);
    Var(const char* val);       // VarList::AddVar(const Var& v) will change to bool if no this function.
    Var(const wchar_t* val);
    Var(const std::string& val);
    Var(const std::wstring& val);
    bool operator==(const Var& copy) const;

    int GetType() const;
    void SetType(int type);
	std::string SerializeTo();
	void ParseFrom(const std::string& str);

    bool GetBool() const;
    int GetInt() const;
    sint64 GetInt64() const;
    float GetFloat() const;
    double GetDouble() const;
    const char* GetStr() const;
    const wchar_t* GetWstr() const;

    void SetBool(bool val);
    void SetInt(int val);
    void SetInt64(sint64 val);
    void SetFloat(float val);
    void SetDouble(double val);
    void SetStr(const std::string& str);
    void SetWstr(const std::wstring& wstr);
private:
    int m_type;

    union
    {
        bool m_bool;
        int m_int;
        sint64 m_int64;
        float m_float;
        double m_double;
    };

    std::string m_str;
    std::wstring m_wstr;
};


NS_YY_END