/************************************************************************/
/* 
@author:    junliang
@brief:     ini read write
@time:      6/12/2016
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include "core/inc/yy_varlist.h"
#include <vector>
#include <string>
#include <map>

NS_YY_BEGIN

class SectionData
{
public:
    void SetSection(const std::string& strSectionName){ m_section = strSectionName; }
    const char* GetSection(){ return m_section.c_str(); }

    const char* Get(const std::string& key);
    bool IsExist(const std::string& key);
    void Set(const std::string& key, const std::string& val);

    void GetData(std::map<std::string, std::string>& data){ data = m_items; }
private:
    std::string m_section;
    std::map<std::string, std::string> m_items;
};

class IniParser
{
public:
    IniParser(const std::string& file);
    ~IniParser();

    void Load();
    void Save();
    std::string GetFile(){ return m_file; }

    std::string Read(const std::string& section, const std::string& key, const std::string& def="");
    void Write(const std::string& section, const std::string& key, const std::string& val);

    void GetData(std::map<std::string, SectionData>& data);
private:
    void Build(char* buf, int len);
private:
    std::string m_file;
    std::map<std::string, SectionData> m_sections;
};

NS_YY_END