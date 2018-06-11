/************************************************************************/
/*
@author:  junliang
@time:    20130308
@brief:   任意类型的变量数组, 使用范围是短小精悍方便，所以使用栈空间
*/
/************************************************************************/
#ifndef _yy_Var_list_h_
#define _yy_Var_list_h_

#include "reflection/yy_type.h"
#include <string.h>
#include "../inc/yy_var.h"
#include <vector>
NS_YY_BEGIN


#define MAX_VAR_LIST_SIZE 1024*1024
class VarList
{
public:
    VarList();
    ~VarList();
    VarList(bool val);
    VarList(int val);
    VarList(sint64 val);
    VarList(float val);
    VarList(double val);
    VarList(const std::string& val);
    VarList(const std::wstring& val);

    VarList& operator<<(bool val);
    VarList& operator<<(int val);
    VarList& operator<<(sint64 val);
    VarList& operator<<(float val);
    VarList& operator<<(double val);
    VarList& operator<<(const char* val);
    VarList& operator<<(const wchar_t* val);
    VarList& operator<<(const std::string& val);
    VarList& operator<<(const std::wstring& val);
    VarList& operator<<(const Var& v);


	operator std::string() const{
		return SerializeTo();
	}

    uint32 GetCount() const ;
    int GetType(uint32 index) const ;
    bool GetBool(uint32 index) const ;
    int GetInt(uint32 index) const ;
    sint64 GetInt64(uint32 index) const;
    float GetFloat(uint32 index) const;
    double GetDouble(uint32 index) const;
    const char* GetStr(uint32 index) const;
    const wchar_t* GetWstr(uint32 index) const;
    Var GetVar(uint32 index) const;
    Var& GetVar(uint32 index);

	// 序列化到文件时无法查看数据，并且序列化后，反序列化失败，用json来做序列化。
    // 此处参数用string 而不是const char*, 因为字符串中间可能会有空格
    std::string SerializeTo() const;
	void ParseFrom(const std::string& str);

    // 追加
    void Append(const YY::VarList& src, uint32 nstart, uint32 ncnt);
private:
    void AddVar(const Var& Var);

    // serialize
    void WriteVar(char* buf, const Var& v, uint32& nWriteIndex) const;
    void ReadVar(const char* buf, Var& v, uint32& nReadIndex) const;
private:
	std::vector<Var> m_data;
};


NS_YY_END
#endif
