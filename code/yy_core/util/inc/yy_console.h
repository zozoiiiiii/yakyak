/************************************************************************/
/* 
@author:    junliang
@brief:     ����̨����
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

    // ������Ļ�ߴ�
    void SetSize(int height, int width, int logheight);
    // ����ȱʡ��ɫ
    void SetColor(int color);
    
    // ��Ļ���
    bool Clear();
    // ģ�����������Ϣ
    bool Input(const char* info);
    // �����Ϣ����Ļ�ϲ�
    bool WriteLog(const char* str, const char* color);
    // �����Ϣ����Ļ�²�
    bool WriteOut(const char* str, const char* color);

    // ����������ʾ��
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