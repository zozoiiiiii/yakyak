/************************************************************************/
/* 
@author:    junliang
@brief:     控制台窗口
@time:      4/26/2016
*/
/************************************************************************/
#pragma once

#include "reflection/yy_type.h"
#include "core/inc/FastDelegate.h"
#include <windows.h>
#include <vector>
#include <string>

NS_YY_BEGIN

class Console
{
public:
    Console();
    virtual ~Console();

    typedef fastdelegate::FastDelegate1<const char*> cb_cmd;
    void Init(cb_cmd func);
    void Execute();

    // 设置屏幕尺寸
    void SetSize(int height, int width, int logheight);
    // 设置缺省颜色
    void SetColor(int color);
    
    // 屏幕清空
    bool Clear();
    // 模拟键盘输入信息
    bool Input(const char* info);
    // 输出信息到屏幕上部
    bool WriteLog(const char* str, const char* color);
    // 输出信息到屏幕下部
    bool WriteOut(const char* str, const char* color);

    // 命令输入提示符
    void SetPrompt(const char* value);
    const char* GetPrompt() const;
private:
    void WriteLine(int line, const char* string, unsigned int color);
    void RestoreConfig();
    int GetColor(const std::string& color) const;
    int GetCmdRow() const;
    bool ParseInput(const char* info);
    void RecoverInput();
    void WriteInput();
    void InputChar(char ch);
    void DeleteChar();
    void BackDelChar();

private:
    static bool m_bConsoleQuit;
    static BOOL WINAPI ConsoleCtrlHandler(DWORD type);

private:
    cb_cmd m_cb_cmd;
    HWND m_hWnd;
    HANDLE m_hStdOut;
    HANDLE m_hStdIn;
    bool m_bLoaded;
    int m_nHeight;
    int m_nWidth;
    int m_nLogHeight;
    int m_nForeColor;
    int m_nCount;
    char m_strBuffer[256];
    std::string m_strPrompt;
    std::vector<std::string> m_SaveInput;
    int m_nSavePos;
    int m_nInputPos;
};
NS_YY_END