#include "../inc/yy_varlist.h"
#include "../inc/yy_exception.h"
NS_YY_BEGIN


VarList::VarList()
{
}

VarList::~VarList()
{}

VarList::VarList(bool val)
{
    AddVar(val);
}

VarList::VarList(int val)
{
    AddVar(val);
}

VarList::VarList(sint64 val)
{
    AddVar(val);
}

VarList::VarList(float val)
{
    AddVar(val);
}

VarList::VarList(double val)
{
    AddVar(val);
}

VarList::VarList(const std::string& val)
{
    AddVar(val);
}

VarList::VarList(const std::wstring& val)
{
    AddVar(val);
}

void VarList::AddVar(const Var& v)
{
    m_data.push_back(v);
}

VarList& VarList::operator<<(bool val)
{
    AddVar(val);
    return *this;
}

VarList& VarList::operator<<(int val)
{
    AddVar(val);
    return *this;
}
VarList& VarList::operator<<(sint64 val)
{
    AddVar(val);
    return *this;
}

VarList& VarList::operator<<(float val)
{
    AddVar(val);
    return *this;
}

VarList& VarList::operator<<(double val)
{
    AddVar(val);
    return *this;
}


VarList& VarList::operator<<(const char* val)
{
    AddVar(val);
    return *this;
}

VarList& VarList::operator<<(const wchar_t* val)
{
    AddVar(val);
    return *this;
}

VarList& VarList::operator<<(const std::string& val)
{
    AddVar(val);
    return *this;
}

VarList& VarList::operator<<(const std::wstring& val)
{
    AddVar(val);
    return *this;
}


VarList& VarList::operator<<(const Var& v)
{
    AddVar(v);
    return *this;
}


uint32 VarList::GetCount() const 
{
    return m_data.size();
}

int VarList::GetType(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);

    return m_data[index].GetType();
}

bool VarList::GetBool(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index].GetBool();
}

int VarList::GetInt(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index].GetInt();
}

sint64 VarList::GetInt64(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index].GetInt64();
}

float VarList::GetFloat(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index].GetFloat();
}

double VarList::GetDouble(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index].GetDouble();
}

const char* VarList::GetStr(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index].GetStr();
}

const wchar_t* VarList::GetWstr(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index].GetWstr();
}

Var VarList::GetVar(uint32 index) const
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index];
}


Var& VarList::GetVar(uint32 index)
{
    throw_assert (index >=0 && index < GetCount(), "index check."<<index);
    return m_data[index];
}

std::string VarList::SerializeTo() const
{
    //const char* pbuf = (const char*)&m_data;
    //return std::string(pbuf, sizeof(int) + sizeof(Var)*GetCount());
    char* pTmpBuf =  new char[MAX_VAR_LIST_SIZE];
    uint32 nWriteIndex=0;

    int nCnt = GetCount();

    int* pCnt = (int*)pTmpBuf;
    *pCnt = nCnt;
    nWriteIndex += sizeof(int);

    for(int i=0; i<nCnt; i++)
    {
        const Var& v = m_data[i];
        WriteVar(pTmpBuf, v, nWriteIndex);
    }

    std::string tmpStr(pTmpBuf, nWriteIndex);
    delete[] pTmpBuf;
    return tmpStr;
}


void VarList::ParseFrom(const std::string& buf)
{
    // ¿½±´ÄÚÈÝ
    //char* pbuf = (char*)&m_data;

    // Ç¿×ª
    //memcpy(pbuf, buf, len);

    uint32 nReadIndex=0;
    const int* pCnt = (const int*)buf.c_str();
    nReadIndex+=sizeof(int);

    int nCnt = *pCnt;
    for(int i=0; i<nCnt; i++)
    {
        Var v;
        ReadVar(buf.c_str(), v, nReadIndex);
        AddVar(v);
    }

	throw_assert(nReadIndex == buf.length(), "length check.");
}


void VarList::Append(const YY::VarList& src, uint32 nstart, uint32 ncnt)
{
    throw_assert(nstart < src.GetCount(), "start pos error.");
    uint32 nend = nstart + ncnt;

    throw_assert(nend <= src.GetCount(), "end pos error.");
    for (uint32 i = nstart; i < nend; ++i)
    {
        Var tmp;
        int ntype = src.GetType(i);
        switch (ntype)
        {
        case YVT_BOOL:
            tmp = Var(src.GetBool(i));
            break;
        case YVT_SINT32:
            tmp = Var(src.GetInt(i));
            break;
        case YVT_SINT64:
            tmp = Var(src.GetInt64(i));
            break;
        case YVT_FLOAT:
            tmp = Var(src.GetFloat(i));
            break;
        case YVT_DOUBLE:
            tmp = Var(src.GetDouble(i));
            break;
        case YVT_STRING:
            tmp = Var(src.GetStr(i));
            break;
        case YVT_WSTRING:
            tmp = Var(src.GetWstr(i));
            break;
        default:
            throw_assert(false, "not support type");
            break;
        }

        AddVar(tmp);
    }
}



void VarList::WriteVar(char* buf, const Var& src, uint32& nWriteIndex) const
{
    int* nType = (int*)(buf+nWriteIndex);
    *nType = src.GetType();
    nWriteIndex += sizeof(int);

    switch (src.GetType())
    {
    case YVT_BOOL:
        {
            bool* pVal = (bool*)(buf+nWriteIndex);
            *pVal = src.GetBool();
            nWriteIndex += sizeof(bool);
        }
        break;
    case YVT_SINT32:
        {
            int* pVal = (int*)(buf+nWriteIndex);
            *pVal = src.GetInt();
            nWriteIndex += sizeof(int);
        }
        break;
    case YVT_SINT64:
        {
            sint64* pVal = (sint64*)(buf+nWriteIndex);
            *pVal = src.GetInt64();
            nWriteIndex += sizeof(sint64);
        }
        break;
    case YVT_FLOAT:
        {
            float* pVal = (float*)(buf+nWriteIndex);
            *pVal = src.GetFloat();
            nWriteIndex += sizeof(float);
        }
        break;
    case YVT_DOUBLE:
        {
            double* pVal = (double*)(buf+nWriteIndex);
            *pVal = src.GetDouble();
            nWriteIndex += sizeof(double);
        }
        break;
    case YVT_STRING:
        {
            uint32 nStrLen = (uint32)strlen(src.GetStr());
            uint32* pLen = (uint32*)(buf+nWriteIndex);
            *pLen = nStrLen;
            nWriteIndex += sizeof(uint32);

            strcpy(buf+nWriteIndex, src.GetStr());
            nWriteIndex += nStrLen;
        }
        break;
    case YVT_WSTRING:
        {
            uint32 nStrLen = (uint32)wcslen(src.GetWstr());
            uint32* pLen = (uint32*)(buf+nWriteIndex);
            *pLen = nStrLen;
            nWriteIndex += sizeof(uint32);

            wchar_t* nVal = (wchar_t*)(buf+nWriteIndex);
            wcscpy(nVal, src.GetWstr());
            nWriteIndex += nStrLen;
        }
        break;
    default:
        throw_assert(false, "not support type");
        break;
    }
}


void VarList::ReadVar(const char* buf, Var& src, uint32& nReadIndex) const
{
    const int* nType = (const int*)(buf+nReadIndex);
    src.SetType(*nType);
    nReadIndex += sizeof(int);

    switch (src.GetType())
    {
    case YVT_BOOL:
        {
            const bool* pVal = (const bool*)(buf+nReadIndex);
            src.SetBool(*pVal);
            nReadIndex += sizeof(bool);
        }
        break;
    case YVT_SINT32:
        {
            const int* pVal = (const int*)(buf+nReadIndex);
            src.SetInt(*pVal);
            nReadIndex += sizeof(int);
        }
        break;
    case YVT_SINT64:
        {
            sint64* pVal = (sint64*)(buf+nReadIndex);
            src.SetInt64(*pVal);
            nReadIndex += sizeof(sint64);
        }
        break;
    case YVT_FLOAT:
        {
            float* pVal = (float*)(buf+nReadIndex);
            src.SetFloat(*pVal);
            nReadIndex += sizeof(float);
        }
        break;
    case YVT_DOUBLE:
        {
            double* pVal = (double*)(buf+nReadIndex);
            src.SetDouble(*pVal);
            nReadIndex += sizeof(double);
        }
        break;
    case YVT_STRING:
        {
            int* pLen = (int*)(buf+nReadIndex);
            int nStrLen = *pLen;
            nReadIndex += sizeof(int);

            src.SetStr(std::string(buf+nReadIndex, nStrLen));
            nReadIndex += nStrLen;
        }
        break;
    case YVT_WSTRING:
        {
            int* pLen = (int*)(buf+nReadIndex);
            int nStrLen = *pLen;
            nReadIndex += sizeof(int);

            wchar_t* nVal = (wchar_t*)(buf+nReadIndex);
            src.SetWstr(std::wstring(nVal, nStrLen));
            nReadIndex += nStrLen;
        }
        break;
    default:
        throw_assert(false, "not support type");
        break;
    }
}
NS_YY_END
