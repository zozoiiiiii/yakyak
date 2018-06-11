#include "util/inc/yy_file.h"
#include "core/inc/yy_exception.h"
#include "util/inc/yy_memory.h"
#include "util/inc/yy_string.h"
#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>
#include <direct.h>
#include <string>
#include <io.h>
#include <errno.h>




NS_YY_BEGIN


void MkFile(const std::string& file)
{
    FILE* pFile = fopen(file.c_str(), "w");
    throw_assert(NULL != pFile, "create file:" << file <<"errno:%d"<< GetLastError());

    fclose(pFile);
}

void RmFile(const std::string& file)
{
    throw_assert(0 == remove(file.c_str()), "remove file:" << file << "errno:" << GetLastError());
}

bool IsFileExist(const std::string& file)
{
    //00: existence, 02:w, 04:r, 06:wr
    if(-1 == _access(file.c_str(), 0))
    {
        return false;
    }

    return true;
}

bool IsFile(const std::string& file)
{
    struct __stat64 buf;

    // ENOENT, EINVAL
    throw_assert(0 == _stat64(file.c_str(), &buf), "errno:"<<errno);

    if(_S_IFMT&buf.st_mode)
        return true;

    return false;
}

sint64 GetFileSize(const std::string& file)
{
    struct __stat64 buf;

    // ENOENT, EINVAL
    throw_assert(0 == _stat64(file.c_str(), &buf), "errno:"<<errno);

    return buf.st_size;
}


void MkDir(const std::string& val)
{
    std::string tmp_dir = SlashTrim(val, false);
    if(IsDirExist(tmp_dir))
        return;

    size_t len = tmp_dir.length();
    AutoMem<char, 512> tmpDirPath(len+1);
    char* pTmpDir = tmpDirPath.GetBuf();
    strcpy(pTmpDir, tmp_dir.c_str());

    // create the pre dir.
    for(size_t i=0; i<len; i++)
    {
        if('\\' == pTmpDir[i])
        {
            pTmpDir[i] = '\0';

            //check whether dir is exist
            if(IsDirExist(pTmpDir))
            {
                pTmpDir[i] = '\\';
                continue;
            }

            throw_assert(_mkdir(pTmpDir) == 0, "make dir:" << pTmpDir << "error.errno:" << GetLastError());

            pTmpDir[i] = '\\';
        }
    }

    throw_assert(_mkdir(pTmpDir) ==0, "make dir:" << pTmpDir << "error.errno:" << GetLastError());
}

void RmDir(const std::string& dir)
{
    //dir is end of '\\'
    std::string strTmpDirPath = SlashTrim(dir, true);
    strTmpDirPath += "*.*";

    int hasfile=-1;
    std::string path;
    struct _finddatai64_t c_file;
    intptr_t handle = _findfirsti64(strTmpDirPath.c_str(), &c_file);
    if(-1 == handle)
        return;

    do
    {
        if((strcmp(c_file.name, ".") !=0 )
            && (strcmp(c_file.name, "..") != 0))
        {
            path =dir + "\\" + std::string(c_file.name);

            if(c_file.attrib & _A_SUBDIR)
            {
                //递归
                RmDir(path);
            }
            else
            {
                RmFile(path);
            }
        }

        hasfile=_findnexti64(handle, &c_file);
    }
    while(0 == hasfile);

    _findclose(handle);
    throw_assert(-1 != _rmdir(dir.c_str()), "remove dir:" << path << "error. errorno:" << GetLastError());
}


void RenameDir(const std::string& old_name, std::string& new_name)
{
    if(!IsDirExist(old_name))
        return;

    throw_assert(0 == rename(old_name.c_str(), new_name.c_str()), "rename check.");
}

bool IsDir(const std::string& dir)
{
    struct _stat _buf;
    int nRet = _stat(dir.c_str(), &_buf);

    // ENOENT, EINVAL
    throw_assert(0 == nRet, "errno:"<<errno);

    if(_S_IFDIR&_buf.st_mode)
        return true;

    return false;
}

bool IsDirExist(const std::string& dir)
{
    if(-1 == _access(dir.c_str(), 0))
    {
        return false;
    }

    return true;
}

void CopyFile(const std::string& dst, const std::string& src)
{
    //use fopen or iostream to non-platform specific
    //use windows api:copyfile
    if(false == ::CopyFileA(src.c_str(), dst.c_str(), false))
    {
        //copy fail, mayge the dir is not exist.
        std::string str_file(dst);
        int index=(int)str_file.find_last_of('\\');
        std::string str_path=str_file.substr(0, index);
        if(!IsDirExist(str_path.c_str()))
        {
            MkDir(str_path.c_str());
            BOOL ret = ::CopyFileA(src.c_str(), dst.c_str(), false);
            throw_assert(ret, "copy file from:"<<src<<"to "<<dst<<"error.errno:" << GetLastError());
        }
    }
}

void CopyDir(const std::string& dst, const std::string& src)
{
    std::string strDst = SlashTrim(dst, false);
    std::string strSrc = SlashTrim(src, false);

    if(!IsDirExist(strDst))
    {
        MkDir(strDst);
    }

    int hasfile=-1;
    std::string dirpathwc = strSrc + "\\*.*";

    std::string strSubDst;
    std::string strSubSrc;
    struct _finddatai64_t c_file;
    intptr_t ret = _findfirsti64(dirpathwc.c_str(), &c_file);
    if(-1 == ret)
        return;

    do
    {
        if(strcmp(c_file.name, ".")!=0 && strcmp(c_file.name, "..") != 0)
        {
            strSubDst = strDst + "\\" + std::string(c_file.name);
            strSubSrc = strSrc + "\\" + std::string(c_file.name);

            if(c_file.attrib & _A_SUBDIR)
            {
                CopyDir(strSubDst, strSubSrc);
            }
            else
            {
                CopyFile(strSubDst, strSubSrc);
            }
        }

        hasfile=_findnexti64(ret, &c_file);
    }
    while(0 == hasfile);

    _findclose(ret);
}


std::string ExePath()
{
    char buffer[1024];
    GetModuleFileNameA( NULL, buffer, 1024 );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\" );
    return std::string( buffer ).substr( 0, pos);
}

std::string GetPrePath(const std::string& path)
{
    std::string strTmp = SlashTrim(path, false);

    //参数是目录的话,有可能是以分隔符结尾,先确保path不是以分隔符结束的.
    std::string::size_type pos = strTmp.find_last_of( "\\" );
    return strTmp.substr( 0, pos);
}


std::string GetFileNameFromPath(const std::string& path)
{
    std::string strTmp = SlashTrim(path, false);

    //参数是目录的话,有可能是以分隔符结尾,先确保path不是以分隔符结束的.
    std::string::size_type pos = strTmp.find_last_of("\\");
    return strTmp.substr(pos+1);
}


std::string ExeName()
{
    char buffer[1024];
    GetModuleFileNameA( NULL, buffer, 1024 );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\" );
    std::string::size_type pos1 = std::string( buffer ).find_last_of( "." );
    return std::string( buffer ).substr( pos+1, pos1-pos-1);
}
NS_YY_END
