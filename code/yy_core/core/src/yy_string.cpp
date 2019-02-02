#include "core/inc/yy_string.h"
#include "core/inc/yy_exception.h"
#include "core/inc/yy_memory.h"

#include <stdlib.h>
#include <algorithm>
#include <wchar.h>

#ifdef _WIN32
#include <windows.h>
#elif __ANDROID__
#elif __APPLE__
#endif

NS_YY_BEGIN
std::string Int32ToStr(sint32 val)
{
    // integer max : 4294967295l
    char buf[16] = { '\0' };
    sprintf(buf, "%d", val);
    buf[15] = 0;
    return buf;
}

std::string Int64ToStr(sint64 val)
{
    // integer_64 max : 18446744073709551615l
    char buf[32] = { '\0' };

#ifdef _WIN32
    sprintf(buf, "%I64d", val);

#elif __ANDROID__
    sprintf(buf, "%lld", val);

#elif __APPLE__
#endif
    buf[31] = 0;
    return buf;
}

std::string UInt32ToStr(uint32 val)
{
    char buf[16] = {'\0'};
    sprintf(buf, "%u", val);
    buf[15] = 0;
    return buf;
}

std::string UInt64ToStr(uint64 val)
{
    char buf[32] = {'\0'};
#ifdef _WIN32
    sprintf(buf, "%I64u", val);

#elif __ANDROID__
    sprintf(buf, "%llu", val);

#elif __APPLE__
#endif
    buf[31] = 0;
    return buf;
}

std::string FloatToStr(float val)
{
    char buf[32] = {'\0'};
    sprintf( buf, "%f", val);
    buf[31] = 0;
    return buf;
}

std::string DoubleToStr(double val)
{
    char buf[64] = {'\0'};
    sprintf( buf, "%lf", val);
    buf[31] = 0;
    return buf;
}

sint32 StrToInt32(const std::string& val)
{
    return atoi(val.c_str());
}

sint64    StrToInt64(const std::string& val)
{
#ifdef _WIN32
    return _atoi64(val.c_str());
#elif __ANDROID__
    return atoll(val.c_str());
#elif __APPLE__
#endif
}

float StrToFloat(const std::string& val)
{
    return (float)atof(val.c_str());
}

#ifdef _WIN32
std::wstring StrToWStr(const std::string& val, unsigned int codepage)
{
    int len;

    //get unicode length
    len = MultiByteToWideChar(codepage, 0,val.c_str(), -1, NULL,0);

    /*    ERROR_INSUFFICIENT_BUFFER. A supplied buffer size was not large enough, or it was incorrectly set to NULL. 
        ERROR_INVALID_FLAGS. The values supplied for flags were not valid. 
        ERROR_INVALID_PARAMETER. Any of the parameter values was invalid. 
        ERROR_NO_UNICODE_TRANSLATION. Invalid Unicode was found in a string. 
        */
    throw_assert(len>=1, "errno:"<< GetLastError());

    AutoMem<wchar_t, 512> wsBuf(len);
    len = MultiByteToWideChar(codepage, 0, val.c_str(), -1, wsBuf.GetBuf(), len);
    throw_assert(len>=1, "errno:"<<GetLastError());

    return wsBuf.GetBuf();
}

std::string WStrToStr(const std::wstring& val, unsigned int codepage)
{
    int len;

    //get multi byte len
    len = WideCharToMultiByte(codepage, 0, val.c_str(), -1, NULL, 0, NULL, NULL);
    throw_assert(len>=1, "errno:"<<GetLastError());

    AutoMem<char, 512> strBuf(len);

    //unicode convert
    len = WideCharToMultiByte (codepage, 0, val.c_str(), -1, strBuf.GetBuf(), len, NULL,NULL);
    throw_assert(len>=1, "errno:"<<GetLastError());

    return strBuf.GetBuf();
}
#elif __ANDROID__

inline size_t GetUTF8ToWideStrLen(const char* s)
{
    size_t src_len = strlen(s);

    if (0 == src_len)
    {
        return 1;
    }

    size_t count = 0;
    size_t pos = 0;

    while (pos < src_len)
    {
        unsigned char c = s[pos];

        if ((c & 0x80) == 0)
        {
            count += 1;
            pos += 1;
        }
        else if ((c & 0xE0) == 0xC0)
        {
            count += 1;
            pos += 2;
        }
        else if ((c & 0xF0) == 0xE0)
        {
            count += 1;
            pos += 3;
        }
        else if ((c & 0xF8) == 0xF0)
        {
            count += 1;
            pos += 4;
        }
        else
        {
            count += 1;
            pos += 5;
        }
    }

    return count + 1;
}


inline size_t GetWideStrToUTF8Len(const wchar_t* ws)
{
    size_t src_len = wcslen(ws);

    if (0 == src_len)
    {
        return 1;
    }

    size_t count = 0;

    for (size_t i = 0; i < src_len; ++i)
    {
        wchar_t wch = ws[i];

        if (wch < 0x80)	// 0000 - 007F
        {
            count += 1;
        }
        else if (wch < 0x800) // 0080 - 07FF
        {
            count += 2;
        }
        else if (wch < 0x10000)	// 0800 - FFFF
        {
            count += 3;
        }
        else if (wch < 0x110000) //	0001 0000 - 0010 FFFF
        {
            count += 4;
        }
        else
        {
            count += 5;
        }
    }

    return count + 1;
}


inline const wchar_t* UTF8ToWideStr(const char* info, wchar_t* buf, size_t byte_size)
{
    // android ndk not support wcstombs, mbstowcs functions
    //setlocale(LC_ALL, "en_US.UTF-8");

    //const size_t len = byte_size / sizeof(wchar_t);
    //size_t res = mbstowcs(buf, info, len);

    //if (res == (size_t)(-1))
    //{
    //    buf[0] = 0;
    //}
    //else if (res == len)
    //{
    //    buf[len - 1] = 0;
    //}

    //setlocale(LC_ALL, "");

    //return buf;
    
    size_t src_len = strlen(info);

    if (0 == src_len)
    {
        buf[0] = 0;
        return buf;
    }

    size_t max_len = byte_size / sizeof(wchar_t) - 1;
    size_t count = 0;
    size_t pos = 0;

    while (pos < src_len)
    {
        if (count >= max_len)
        {
            break;
        }

        wchar_t wchar;
        unsigned char c = info[pos];

        if ((c & 0x80) == 0) // 0xxx xxxx
        {
            wchar = info[pos];
            pos += 1;
        }
        else if ((c & 0xE0) == 0xC0) // 110x xxxx 10xx xxxx
        {
            wchar = (info[pos + 0] & 0x3F) << 6;
            wchar |= (info[pos + 1] & 0x3F);
            pos += 2;
        }
        else if ((c & 0xF0) == 0xE0) // 1110 xxxx 10xx xxxx 10xx xxxx
        {
            wchar = (info[pos + 0] & 0x1F) << 12;
            wchar |= (info[pos + 1] & 0x3F) << 6;
            wchar |= (info[pos + 2] & 0x3F);
            pos += 3;
        }
        else if ((c & 0xF8) == 0xF0) // UCS4: 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
        {
            wchar = (info[pos + 0] & 0x0F) << 18;
            wchar |= (info[pos + 1] & 0x3F) << 12;
            wchar |= (info[pos + 2] & 0x3F) << 6;
            wchar |= (info[pos + 3] & 0x3F);
            pos += 4;
        }
        else // UCS4: 1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx 
        {
            wchar = (info[pos + 0] & 0x07) << 24;
            wchar |= (info[pos + 1] & 0x3F) << 18;
            wchar |= (info[pos + 2] & 0x3F) << 12;
            wchar |= (info[pos + 3] & 0x3F) << 6;
            wchar |= (info[pos + 4] & 0x3F);
            pos += 5;
        }

        buf[count++] = wchar;
    }

    buf[count] = 0;

    return buf;
}


inline const char* WideStrToUTF8(const wchar_t* info, char* buf, size_t byte_size)
{
    // android ndk not support wcstombs, mbstowcs functions
    //setlocale(LC_ALL, "en_US.UTF-8");

    //size_t res = wcstombs(buf, info, byte_size);

    //if (res == (size_t)(-1))
    //{
    //buf[0] = 0;
    //}
    //else if (res == byte_size)
    //{
    //buf[byte_size - 1] = 0;
    //}

    //setlocale(LC_ALL, "");

    //return buf;
    
    size_t src_len = wcslen(info);

    if (0 == src_len)
    {
        buf[0] = 0;
        return buf;
    }

    size_t max_len = byte_size - 1;
    size_t count = 0;

    for (size_t i = 0; i < src_len; ++i)
    {
        wchar_t wch = info[i];

        if (wch < 0x80)	// 0xxx xxxx
        {
            if ((count + 1) > max_len)
            {
                break;
            }

            buf[count++] = (char)wch;
        }
        else if (wch < 0x800) // 110x xxxx 10xx xxxx
        {
            if ((count + 2) > max_len)
            {
                break;
            }

            buf[count++] = 0xC0 | (wch >> 6);
            buf[count++] = 0x80 | (wch & 0x3F);
        }
        else if (wch < 0x10000)	// 1110 xxxx 10xx xxxx 10xx xxxx
        {
            if ((count + 3) > max_len)
            {
                break;
            }

            buf[count++] = 0xE0 | (wch >> 12);
            buf[count++] = 0x80 | ((wch >> 6) & 0x3F);
            buf[count++] = 0x80 | (wch & 0x3F);
        }
        else if (wch < 0x110000) // UCS4: 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
        {
            if ((count + 4) > max_len)
            {
                break;
            }

            buf[count++] = 0xF0 | (wch >> 18);
            buf[count++] = 0x80 | ((wch >> 12) & 0x3F);
            buf[count++] = 0x80 | ((wch >> 6) & 0x3F);
            buf[count++] = 0x80 | (wch & 0x3F);
        }
        else // UCS4: 1111 10xx 10xx xxxx 10xx xxxx 10xx xxxx 10xx xxxx
        {
            if ((count + 5) > max_len)
            {
                break;
            }

            buf[count++] = 0xF8 | (wch >> 24);
            buf[count++] = 0x80 | ((wch >> 18) & 0x3F);
            buf[count++] = 0x80 | ((wch >> 12) & 0x3F);
            buf[count++] = 0x80 | ((wch >> 6) & 0x3F);
            buf[count++] = 0x80 | (wch & 0x3F);
        }
    }

    buf[count] = 0;

    return buf;
}

#elif __APPLE__
#endif




std::wstring    Utf8ToWStr(const std::string& val)
{
#ifdef _WIN32
    return StrToWStr(val, CP_UTF8);

#elif __ANDROID__

    size_t size = GetUTF8ToWideStrLen(val.c_str());
    AutoMem<wchar_t, 256> auto_buf(size);
    wchar_t* buffer = auto_buf.GetBuf();
    UTF8ToWideStr(val.c_str(), buffer, size * sizeof(wchar_t));

    return std::wstring(buffer);

#elif __APPLE__
#endif
}

std::string        WStrToUtf8(const std::wstring& val)
{
#ifdef _WIN32
    return WStrToStr(val, CP_UTF8);
#elif __ANDROID__
    size_t size = GetWideStrToUTF8Len(val.c_str());

    AutoMem<char, 256> auto_buf(size);

    char* buffer = auto_buf.GetBuf();

    WideStrToUTF8(val.c_str(), buffer, size);

    return std::string(buffer);

#elif __APPLE__
#endif
}

//std::wstring    AnsiToWStr(const std::string& val)
//{
//    return StrToWStr(val, CP_ACP);
//}
//
//std::string        WStrToAnsi(const std::wstring& val)
//{
//    return WStrToStr(val, CP_ACP);
//}
//
//std::string AnsiToUtf8(const std::string& val)
//{
//    std::wstring wstr=AnsiToWStr(val);
//    return WStrToUtf8(wstr.c_str());
//}
//
//std::string Utf8ToAnsi(const std::string& val)
//{
//    std::wstring wstr=Utf8ToWStr(val);
//    return WStrToAnsi(wstr.c_str());
//}

std::string ToUpper(const std::string& str)
{
    AutoMem<char, 256> auto_mem(str.length() + 1);

    char * data = auto_mem.GetBuf();
    strcpy(data, str.c_str());
    for (int i = 0; i < auto_mem.GetLen(); i++)
    {
        *data = toupper((unsigned char)*data);
    }
    return data;
}

std::string ToLower(const std::string& str)
{
    AutoMem<char, 256> auto_mem(str.length() + 1);
    char * data = auto_mem.GetBuf();
    strcpy(data, str.c_str());
    for (int i = 0; i < auto_mem.GetLen(); i++)
    {
        *data = tolower((unsigned char)*data);
    }
    return data;
}

std::string SlashTrim(const std::string& val, bool slash_end)
{
    int strLen = (int)val.length();
    AutoMem<char, 512> auto_mem(strLen + 2);
    char* data = auto_mem.GetBuf();
    strcpy(data, val.c_str());

    // "/" 统一转换成"\\"
    for(int i=0; i<strLen; i++)
    {
        char tmp=*(data+i);
        if('/'==tmp)
            data[i]='\\';
        else
            data[i]=tmp;
    }

    // should end with "\\"
    if(slash_end)
    {
        if(data[strLen-1] != '\\')
        {
            data[strLen]='\\';
            data[strLen+1]='\0';
        }
        else
        {
            data[strLen]='\0';
        }
    }
    // no need to end with "\\"
    else    
    {
        if(data[strLen-1] != '\\')
        {
            data[strLen]='\0';
        }
        else
        {
            data[strLen-1]='\0';
        }
    }


    return data;
}


// 去除str字符串中前后的指定字符
std::string Trim(const std::string& str, const char ch)
{
    if (str.empty())
    {
        return "";
    }

    if (str[0] != ch && str[str.size()-1] != ch)
    {
        return str;
    }

    size_t pos_begin = str.find_first_not_of(ch, 0);
    size_t pos_end = str.find_last_not_of(ch, str.size());

    if (pos_begin == std::string::npos || pos_end == std::string::npos)
    {
        //这表示这个字符串里的字符全都是ch组成，把它设置为空串
        return "";
    }

    return str.substr(pos_begin, pos_end - pos_begin + 1);
}


void    StrReplace(std::string& str, const std::string& old_value, const std::string& new_value)
{
    while(true)
    {
        std::string::size_type   pos = str.find(old_value);
        if(std::string::npos == pos)
        {
            break;
        }

        str.replace(pos,old_value.length(),new_value);
    }
}

void Split(std::vector<std::string>& result, const std::string & str, const std::string& delims)
{
    if (0 == str.compare(""))
    {
        return;
    }

    size_t start, pos;
    start = 0;
    do 
    {
        pos = str.find(delims, start);
        if (pos == std::string::npos)
        {
            std::string tmp_str = str.substr(start);
            tmp_str = Trim(tmp_str, 10); // 去掉回车
            tmp_str = Trim(tmp_str, 13); // 去掉换行
            tmp_str = Trim(tmp_str, 32); // 去掉空格
            result.push_back(tmp_str);
            break;
        }
        else
        {
            std::string tmp_str = str.substr(start, pos - start);
            tmp_str = Trim(tmp_str, 10); // 去掉回车
            tmp_str = Trim(tmp_str, 13); // 去掉换行
            tmp_str = Trim(tmp_str, 32); // 去掉空格
            result.push_back(tmp_str);
            start = pos + delims.length();
        }

    } while (pos != std::string::npos);
    return;
}

bool CheckBigEndien ()
{
    sint8 swaptest[2] = {1,0};

    // 0x0001: litle endien
    // 0x0100: big endien
    if ( *(short *)swaptest == 1)
        return false;
    else
        return true;
}


int GetBit(int Data, int Pos)
{
    if(Pos<0 || Pos>31)
        return -1;

    if((Data & (1<<Pos)) == 0)
        return 0;
    else
        return 1;
}

int SetBit(int & Data, int Pos, int Flag)
{
    if(Pos<0 || Pos>31)
        return 0;

    if(Flag == GetBit(Data, Pos))
        return 0;

    else if(Flag == 0)
    {
        Data = Data & (~(1<<Pos));    
        return 1;
    }
    else if(Flag == 1)
    {
        Data = Data | (1<<Pos);
        return 1;
    }
    else
        return 0;
}

NS_YY_END