/************************************************************************/
/*
@author:  junliang
@brief:   file
@time:    20130510
*/
/************************************************************************/
#ifndef _yy_file_h_
#define _yy_file_h_

#include "reflection/yy_type.h"
#include <string>
NS_YY_BEGIN

#define MAX_FILEPATHLEN 1024
#define MAX_DIRLEN 1024
#define MAX_PATHLEN 1024

void MkFile(const std::string& file);
void RmFile(const std::string& file); // will not backup in recycle bin.
bool IsFileExist(const std::string& file);
bool IsFile(const std::string& file);
sint64 GetFileSize(const std::string& file);
void CopyFile(const std::string& dst, const std::string& src);

void MkDir(const std::string& dir);
void RmDir(const std::string& dir); // will not backup in recycle bin.
void RenameDir(const std::string& old_name, std::string& new_name);
bool IsDir(const std::string& dir);
bool IsDirExist(const std::string& dir);
void CopyDir(const std::string& dst, const std::string& src);

//��õ�ǰexe����Ŀ¼, ����/, \\��β
std::string ExePath();

/*
//�����һ��Ŀ¼, �����������ļ���,������·����; 
//f:\misc\util_file.h -> f:\misc
//f:\inc\misc\ -> f:\inc
//f:\inc\misc  -> f:\inc
*/
std::string GetPrePath(const std::string& path);
std::string GetFileNameFromPath(const std::string& path);

//��õ�ǰexe������, ��������׺
std::string ExeName();

NS_YY_END
#endif