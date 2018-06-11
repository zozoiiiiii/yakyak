/************************************************************************/
/* 
@author:    junliang
@brief:     string utility func
@time:      8/26/2016
*/
/************************************************************************/
#pragma once

#include "core/inc/yy_var.h"
#include <vector>


NS_YY_BEGIN
std::string Int32ToStr(sint32 val);
std::string Int64ToStr(sint64 val);
std::string UInt32ToStr(uint32 val);
std::string UInt64ToStr(uint64 val);
std::string FloatToStr(float val);
std::string DoubleToStr(double val);

sint32 StrToInt32(const std::string& val);
sint64 StrToInt64(const std::string& val);
float StrToFloat(const std::string& val);

std::wstring Utf8ToWStr(const std::string& val);
std::string WStrToUtf8(const std::wstring& val);

// android ndk not support? maybe find solution in cocos2dx in future.
//std::wstring AnsiToWStr(const std::string& val);
//std::string WStrToAnsi(const std::wstring& val);
//std::string AnsiToUtf8(const std::string& val);
//std::string Utf8ToAnsi(const std::string& val);


std::string ToUpper(const std::string& str);
std::string ToLower(const std::string& str);

// windows:"\\", linux: "/"
std::string SlashTrim(const std::string& val, bool slash_end=false);
std::string Trim(const std::string& str, const char ch = ' ');
void StrReplace(std::string& str, const std::string& old_value, const std::string& new_value);
void Split(std::vector<std::string>& paths, const std::string & szinput, const std::string& delims);

//1. little endian: high|little
//2. big endian: low|high
bool    CheckBigEndien();
sint32 GetBit(sint32 data, sint32 pos);
sint32 SetBit(sint32 & data, sint32 pos, sint32 flag);


NS_YY_END