/************************************************************************/
/*
@author:  junliang
@brief:   process
@time:    20130510
*/
/************************************************************************/
#ifndef _yy_process_h_
#define _yy_process_h_
#include "reflection/yy_type.h"
NS_YY_BEGIN

/*
解析命令行参数，例子：
std::string str_cmdline = getcommandlinea();
char szbuff[1024];
const char* argv[100];
int argc = split_cmdline(strcmdline.c_str(), szbuff, sizeof(szbuff), argv, 100);
*/
int SplitCmdLine(const char* cmd, char* buf, int size, const char* args[], int arg_max);

// 通过命令行方式启动子进程， ishide表示是否隐藏子进程窗口, 返回子进程id
int StartProcess(const char* _cmdline, bool _hide=false);
void StopProcess(int process_id);
int FindProcess(const char* path);
bool ProcessExist(int process_id);
int GetCurProcessID();


NS_YY_END
#endif