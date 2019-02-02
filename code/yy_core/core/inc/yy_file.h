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

#ifdef _WIN32

#elif __ANDROID__

FILE* android_fopen(const char* fname, const char* mode);
#define fopen(name, mode) android_fopen(name, mode)

#elif __APPLE__
#endif


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

std::string ExePath();

//f:\misc\util_file.h -> f:\misc
//f:\inc\misc\ -> f:\inc
//f:\inc\misc  -> f:\inc
std::string GetPrePath(const std::string& path);
std::string GetFileNameFromPath(const std::string& path);

NS_YY_END
#endif