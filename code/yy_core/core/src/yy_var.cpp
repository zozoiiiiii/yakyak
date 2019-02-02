#include "../inc/yy_var.h"
#include "../inc/yy_exception.h"
#include "../inc/yy_string.h"
#include <math.h>
NS_YY_BEGIN

Var::Var() :m_type(YVT_UNKNOWN){}
Var::Var(bool val) :m_type(YVT_BOOL){ m_bool = val;}
Var::Var(int val) :m_type(YVT_SINT32), m_int(val){}
Var::Var(sint64 val) : m_type(YVT_SINT64), m_int64(val){}
Var::Var(float val) : m_type(YVT_FLOAT), m_float(val){}
Var::Var(double val) : m_type(YVT_DOUBLE), m_double(val){}
Var::Var(const char* val) : m_type(YVT_STRING)
{
    SetStr(val);
}

Var::Var(const wchar_t* val) :m_type(YVT_WSTRING)
{
    SetWstr(val);
}

Var::Var(const std::string& val) : m_type(YVT_STRING)
{
    SetStr(val);
}

Var::Var(const std::wstring& val) :m_type(YVT_WSTRING)
{
    SetWstr(val);
}


bool Var::operator==(const Var& copy) const
{
    if (GetType() != copy.GetType())
        return false;

    switch (GetType())
    {
    case YVT_SINT32:
        return GetInt() == copy.GetInt();
    case YVT_SINT64:
        return GetInt64() == copy.GetInt64();
    case YVT_FLOAT:
            return fabs( GetFloat() - copy.GetFloat() ) <= 0.0001;
    case YVT_DOUBLE:
            return fabs( GetDouble() - copy.GetDouble() ) <= 0.0001;
    case YVT_STRING:
        return m_str == copy.m_str;
    case YVT_WSTRING:
        return m_wstr == copy.m_wstr;
    default:
        return false;
    }

    return false;
}

int Var::GetType() const
{
    return m_type;
}

void Var::SetType(int type)
{
    throw_assert(type > YVT_UNKNOWN && type <= YVT_MAX, "var type error:"<<type);

    m_type = type;
}

bool Var::GetBool() const
{
    throw_assert (m_type == YVT_BOOL, "type check.");

    return m_bool;
}

int Var::GetInt() const
{
    throw_assert (m_type == YVT_SINT32, "type check.");

    return m_int;
}

sint64 Var::GetInt64() const
{
    throw_assert(m_type == YVT_SINT64, "type check.");
    return m_int64;
}

float Var::GetFloat() const
{
    throw_assert (m_type == YVT_FLOAT, "type check.");
    return m_float;
}

double Var::GetDouble() const
{
    throw_assert (m_type == YVT_DOUBLE, "type check.");
    return m_double;
}

const char* Var::GetStr() const
{
    throw_assert (m_type == YVT_STRING, "type check.");
    return m_str.c_str();
}

const wchar_t* Var::GetWstr() const
{
    throw_assert (m_type == YVT_WSTRING, "type check.");
    return m_wstr.c_str();
}


void Var::SetBool(bool val)
{
    throw_assert (m_type == YVT_BOOL, "type check.");
    m_bool = val;
}

void Var::SetInt(int val)
{
    throw_assert (m_type == YVT_SINT32, "type check.");
    m_int = val;
}

void Var::SetInt64(sint64 val)
{
    throw_assert (m_type == YVT_SINT64, "type check.");
    m_int64 = val;
}

void Var::SetFloat(float val)
{
    throw_assert (m_type == YVT_FLOAT, "type check.");
    m_float = val;
}

void Var::SetDouble(double val)
{
    throw_assert (m_type == YVT_DOUBLE, "type check.");
    m_double = val;
}

void Var::SetStr(const std::string& str)
{
    throw_assert (m_type == YVT_STRING, "type check.");
	m_str = str;
}

void Var::SetWstr(const std::wstring& wstr)
{
    throw_assert (m_type == YVT_WSTRING, "type check.");
	m_wstr = wstr;
}



std::string Var::SerializeTo()
{
	switch (m_type)
	{
	case YVT_BOOL:
	{
		if (GetBool())
			return "true";
		else
			return "false;";
	}
	break;
	case YVT_SINT32:
		return Int32ToStr(GetInt());
	case YVT_SINT64:
		return Int64ToStr(GetInt64());
	case YVT_FLOAT:
		return FloatToStr(GetFloat());
	case YVT_DOUBLE:
		return DoubleToStr(GetDouble());
	case YVT_STRING:
		return  GetStr();
	case YVT_WSTRING:
		return WStrToUtf8(GetWstr());
	default:
		throw_assert(false, "not support");
	}
}

void Var::ParseFrom(const std::string& str)
{
	switch (m_type)
	{
	case YVT_BOOL:
	{
		bool bValue = true;
		if (str == "true" || str == "TRUE")
			bValue = true;
		else
			bValue = false;
		SetBool(bValue);
		return;
	}
	break;
	case YVT_SINT32:
	{
		sint32 iValue = StrToInt32(str);
		SetInt(iValue);
		return;
	}
	case YVT_SINT64:
	{
		sint64 i64Value = StrToInt64(str);
		SetInt64(i64Value);
		return;
	}
	case YVT_FLOAT:
	{
		float fValue = StrToFloat(str);
		SetFloat(fValue);
		return;
	}
	case YVT_DOUBLE:
	{
		double dValue = StrToFloat(str);
		SetDouble(dValue);
		return;
	}
	case YVT_STRING:
	{
		SetStr(str);
		return;
	}
	case YVT_WSTRING:
	{
		std::wstring wstr = Utf8ToWStr(str);
		SetWstr(wstr);
		return;
	}
	default:
		throw_assert(false, "not support");
	}
}

NS_YY_END
