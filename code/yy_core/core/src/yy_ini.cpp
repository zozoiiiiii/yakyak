#include "util/inc/yy_ini.h"
#include "core/inc/yy_exception.h"

NS_YY_BEGIN

const char* SectionData::Get(const std::string& key)
{
    if (!IsExist(key))
        return "";

    return m_items[key].c_str();
}

bool SectionData::IsExist(const std::string& key)
{
    std::map<std::string, std::string>::iterator itor = m_items.find(key);
    if (itor == m_items.end())
        return false;

    return true;
}


void SectionData::Set(const std::string& key, const std::string& val)
{
    m_items[key] = val;
}

IniParser::IniParser(const std::string& file):m_file(file){}
IniParser::~IniParser(){}

void IniParser::Load()
{
    m_sections.clear();

    FILE* fp = fopen(m_file.c_str(), "rb");
    throw_assert(NULL != fp, "file is not exist." << m_file);

    size_t nFileSize = 0;
    fseek(fp, 0, SEEK_END);
    nFileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    size_t nBufferSize = nFileSize + 2;

    char* buffer = new char[nBufferSize];
    memset(buffer, 0, nBufferSize);

    int nSize = fread(buffer, sizeof(char), nFileSize, fp);
    throw_assert(nSize == nFileSize, "file read error.");
    fclose(fp);

    buffer[nFileSize] = '\n';
    buffer[nFileSize + 1] = 0;

    Build(buffer, nBufferSize);
    delete []buffer;
}

void IniParser::Build(char* buffer, int buffer_size)
{
    size_t char_num = 0;
    bool section_begin = false;
    char* line = buffer;

    std::string section_name;

    for (char* p = buffer; *p; ++p)
    {
        if ((*p == '\r') || (*p == '\n'))
        {
            *p = 0;

            section_begin = false;

            if (0 == char_num)
            {
                continue;
            }

            if (line[0] == ';')
            {
                // 忽略注释
                char_num = 0;
                continue;
            }

            if ((char_num >= 2) && (line[0] == '/') && (line[1] == '/'))
            {
                // 忽略注释
                char_num = 0;
                continue;
            }

            // 清除后面多余的空格
            char* cur = line + char_num - 1;

            while (cur >= line)
            {
                if ((*cur == ' ') || (*cur == '    '))
                {
                    *cur = 0;
                    --cur;
                }
                else
                {
                    break;
                }
            }

            char* equal = strchr(line, '=');

            if (NULL == equal)
            {
                SectionData& st = m_sections[section_name];
                st.Set(line, "");
            }
            else
            {
                *equal = 0;

                // 键名
                char* key = line;

                // 清除键名后多余的空格
                char* temp_cur = equal - 1;
                while (temp_cur >= key)
                {
                    if ((*temp_cur == ' ') || (*temp_cur == '    '))
                    {
                        *temp_cur = 0;
                        --temp_cur;
                    }
                    else
                    {
                        break;
                    }
                }

                // 键值
                char* val = equal + 1;

                // 清除键值前多余的空格
                while (*val)
                {
                    if ((*val == ' ') || (*val == '    '))
                    {
                        *val = 0;
                        ++val;
                    }
                    else
                    {
                        break;
                    }
                }

                SectionData& st = m_sections[section_name];
                st.Set(key,val);
            }


            char_num = 0;

            continue;
        }
        else if (*p == '[')
        {
            if (0 == char_num)
            {
                section_begin = true;
                *p = 0;
                line = p + 1;
                continue;
            }
        }
        else if (*p == ']')
        {
            if (section_begin)
            {
                *p = 0;

                section_name = line;
                SectionData tmp_section;
                tmp_section.SetSection(line);
                m_sections[line] = tmp_section;

                char_num = 0;
                section_begin = false;
                continue;
            }
        }
        else if ((*p == ' ') || (*p == '    '))
        {
            if (0 == char_num)
            {
                // 忽略起始的空格
                continue;
            }
        }

        if (0 == char_num)
        {
            line = p;
        }

        ++char_num;
    }
}

std::string IniParser::Read(const std::string& section, const std::string& key, const std::string& def)
{
    std::map<std::string, SectionData>::iterator itor = m_sections.find(section.c_str());
    throw_assert (itor != m_sections.end(),"cannot fine section:" << section);

    SectionData& st = itor->second;
    if (!st.IsExist(key))
        return def;

    return st.Get(key);
}

void IniParser::Write(const std::string& section, const std::string& key, const std::string& val)
{
    SectionData& st = m_sections[section];
    st.Set(key, val);
}

void IniParser::Save()
{
    FILE* fp = fopen(m_file.c_str(), "wb");
    throw_assert(NULL != fp, "cannot open file:"<<m_file);

    std::map<std::string, SectionData>::iterator itor = m_sections.begin();
    for (; itor != m_sections.end(); itor++)
    {
        std::string name = itor->first;
        SectionData& st = itor->second;

        fprintf(fp, "[%s]\r\n", name.c_str());

        std::map<std::string, std::string> st_data;
        st.GetData(st_data);
        std::map<std::string, std::string>::iterator itor_item = st_data.begin();
        for (; itor_item != st_data.end(); itor_item++)
        {
            const std::string& key = itor_item->first;
            const std::string& val = itor_item->second;

            fprintf(fp, "%s=%s\r\n", key.c_str(), val.c_str());
        }

        fprintf(fp, "\r\n");
    }

    fclose(fp);
}



void IniParser::GetData(std::map<std::string, SectionData>& data)
{
    data = m_sections;
}

NS_YY_END