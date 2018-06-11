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
���������в��������ӣ�
std::string str_cmdline = getcommandlinea();
char szbuff[1024];
const char* argv[100];
int argc = split_cmdline(strcmdline.c_str(), szbuff, sizeof(szbuff), argv, 100);
*/
int SplitCmdLine(const char* cmd, char* buf, int size, const char* args[], int arg_max);

// ͨ�������з�ʽ�����ӽ��̣� ishide��ʾ�Ƿ������ӽ��̴���, �����ӽ���id
int StartProcess(const char* _cmdline, bool _hide=false);
void StopProcess(int process_id);
int FindProcess(const char* path);
bool ProcessExist(int process_id);
int GetCurProcessID();


NS_YY_END
#endif