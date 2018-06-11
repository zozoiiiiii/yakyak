#include "util/inc/yy_console.h"
#include <assert.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif // !_WIN32_WINNT

#include <string.h>


const unsigned int FORECOLOR_RED = FOREGROUND_RED;
const unsigned int FORECOLOR_GREEN = FOREGROUND_GREEN;
const unsigned int FORECOLOR_BLUE = FOREGROUND_BLUE;
const unsigned int FORECOLOR_LIGHTRED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const unsigned int FORECOLOR_LIGHTGREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const unsigned int FORECOLOR_LIGHTBLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const unsigned int FORECOLOR_GRAY = FOREGROUND_RED 
| FOREGROUND_BLUE | FOREGROUND_GREEN;
const unsigned int FORECOLOR_WHITE = FOREGROUND_RED 
| FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const unsigned int FORECOLOR_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;
const unsigned int FORECOLOR_LIGHTYELLOW = FOREGROUND_RED 
| FOREGROUND_GREEN | FOREGROUND_INTENSITY;


NS_YY_BEGIN

typedef HWND (* GET_Console_WINDOW)(VOID);

bool Console::m_bConsoleQuit = false;

BOOL WINAPI Console::ConsoleCtrlHandler(DWORD type)
{
    switch (type)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        m_bConsoleQuit = true;
        return TRUE;
    }

    return FALSE;
}

Console::Console()
{
    m_bLoaded = false;
    m_hWnd = NULL;
    m_hStdOut = NULL;
    m_hStdIn = NULL;
    m_nHeight = 40;
    m_nWidth = 80;
    m_nLogHeight = 25;
    m_nForeColor = FORECOLOR_GRAY;
    m_strPrompt = "Command> ";
    m_strBuffer[0] = 0;
    m_nCount = 0;
    m_nSavePos = -1;
    m_nInputPos = 0;
    m_cb_cmd = NULL;
}

Console::~Console()
{
    if (m_bLoaded)
    {
        FreeConsole();
    }

    if (m_hWnd != NULL)
    {
        m_hWnd = NULL;
    }
}

void Console::Init(cb_cmd func)
{
    m_cb_cmd = func;

    if (!AllocConsole())
    {
        return;
    }
    
    m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    m_hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    
    SetThreadExecutionState(
        ES_SYSTEM_REQUIRED | ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
    SetConsoleMode(m_hStdIn, 0);
    SetConsoleMode(m_hStdOut, 0);
    SetColor(m_nForeColor);
    SetSize(m_nHeight, m_nWidth, m_nLogHeight);

    HMODULE handle = LoadLibraryA("Kernel32.dll");

    if (handle != NULL)
    {
        GET_Console_WINDOW func = (GET_Console_WINDOW)
            GetProcAddress(handle, "GetConsoleWindow");
        
        if (func != NULL)
        {
            m_hWnd = func();

            if (m_hWnd != NULL)
            {
                ShowWindow(m_hWnd, SW_MAXIMIZE);
            }
        }

        FreeLibrary(handle);
    }
    
    m_bLoaded = true;
}


void Console::SetPrompt(const char* value) 
{ 
    m_strPrompt = value; 
}

const char* Console::GetPrompt() const 
{ 
    return m_strPrompt.c_str(); 
}

void Console::RecoverInput()
{
    assert( (m_nSavePos >= 0) && (m_nSavePos < int(m_SaveInput.size())) );

    strcpy_s(m_strBuffer, sizeof(m_strBuffer), m_SaveInput[m_nSavePos].c_str());
    m_nCount = (int)strlen(m_strBuffer);
    m_nInputPos = m_nCount;
}

void Console::WriteInput()
{
    char s[256] = {0};

    sprintf_s(s, sizeof(s), "%s%s", m_strPrompt.c_str(), m_strBuffer);
    WriteLine(GetCmdRow(), s, m_nForeColor);
    
    COORD coord;
    
    coord.X = (SHORT)(m_strPrompt.length() + m_nInputPos);
    coord.Y = GetCmdRow();
    
    SetConsoleCursorPosition(m_hStdOut, coord);
}

void Console::InputChar(char ch)
{
    assert(m_nInputPos <= m_nCount);
    
    if (m_nCount < (sizeof(m_strBuffer) - 1))
    {
        if (m_nInputPos != m_nCount)
        {
            memmove(m_strBuffer + m_nInputPos + 1, 
                m_strBuffer + m_nInputPos, m_nCount - m_nInputPos);
        }
        
        m_strBuffer[m_nInputPos++] = ch;
        m_strBuffer[++m_nCount] = 0;
    }
}

void Console::DeleteChar()
{
    assert(m_nInputPos <= m_nCount);
    
    if (m_nInputPos != m_nCount)
    {
        memmove(m_strBuffer + m_nInputPos, m_strBuffer + m_nInputPos + 1, 
            m_nCount - m_nInputPos - 1);
        m_strBuffer[--m_nCount] = 0;
    }
}

void Console::BackDelChar()
{
    assert(m_nInputPos <= m_nCount);
    
    if (m_nInputPos == 0)
    {
        return;
    }
    
    if (m_nInputPos != m_nCount)
    {
        memmove(m_strBuffer + m_nInputPos - 1, m_strBuffer + m_nInputPos, 
            m_nCount - m_nInputPos);
    }
    
    m_nInputPos--;
    m_strBuffer[--m_nCount] = 0;
}

void Console::Execute()
{
    DWORD dwRead;
    
    if (!GetNumberOfConsoleInputEvents(m_hStdIn, &dwRead))
    {
        return;
    }
    
    if (0 == dwRead)
    {
        return;
    }

    INPUT_RECORD record[128];

    if (!ReadConsoleInput(m_hStdIn, record, 128, &dwRead))
    {
        return;
    }

    for (DWORD i = 0; i < dwRead; i++)
    {
        if (record[i].EventType != KEY_EVENT)
        {
            continue;
        }
        
        if (!record[i].Event.KeyEvent.bKeyDown)
        {
            continue;
        }

        char ch = record[i].Event.KeyEvent.uChar.AsciiChar;
        
        if (ch == 0)
        {
            unsigned int vk = record[i].Event.KeyEvent.wVirtualKeyCode;
            
            if (vk == VK_LEFT)
            {
                if (m_nInputPos > 0)
                {
                    m_nInputPos--;
                }
            }
            else if (vk == VK_RIGHT)
            {
                if (m_nInputPos < m_nCount)
                {
                    m_nInputPos++;
                }
            }
            else if (vk == VK_UP)
            {
                if (m_nSavePos == -1)
                {
                    if (!m_SaveInput.empty())
                    {
                        m_nSavePos = (int)m_SaveInput.size() - 1;
                        RecoverInput();
                    }
                }
                else if (m_nSavePos > 0)
                {
                    m_nSavePos--;
                    RecoverInput();
                }
            }
            else if (vk == VK_DOWN)
            {
                if (m_nSavePos != -1)
                {
                    if (m_nSavePos < int(m_SaveInput.size() - 1))
                    {
                        m_nSavePos++;
                        RecoverInput();
                    }
                    else
                    {
                        m_nSavePos = -1;
                        m_strBuffer[0] = 0;
                        m_nCount = 0;
                        m_nInputPos = 0;
                    }
                }
            }
            else if (vk == VK_DELETE)
            {
                DeleteChar();
            }
        
            continue;
        }
        
        if ('\r' == ch)
        {
            m_strBuffer[m_nCount] = 0;
            Input(m_strBuffer);
            m_nCount = 0;
            m_nInputPos = 0;
        }
        else if ('\b' == ch)
        {
            BackDelChar();
        }
        else
        {
            InputChar(ch);
            m_nSavePos = -1;
        }
    }

    m_strBuffer[m_nCount] = 0;
    WriteInput();
}

void Console::SetSize(int height, int width, int logheight)
{
    assert( (height >= 10) && (height < 100) );
    assert( (width >= 10) && (width < 256) );
    assert( (logheight >= 2) && (logheight < height) );

    m_nHeight = height;
    m_nWidth = width;
    m_nLogHeight = logheight;
    
    SMALL_RECT rect;

    rect.Left = 0;
    rect.Top = 0;
    rect.Right = width - 1;
    rect.Bottom = height - 1;
    
    SetConsoleWindowInfo(m_hStdOut, FALSE, &rect);
    
    COORD coord = { 0, 0 };

    coord.X = width;
    coord.Y = height;
    
    SetConsoleScreenBufferSize(m_hStdOut, coord);
    Clear();
}

void Console::SetColor(int color)
{
    m_nForeColor = color;
    SetConsoleTextAttribute(m_hStdOut, m_nForeColor);
}

bool Console::Clear()
{
    DWORD dwWritten;
    COORD coord = { 0, 0 };

    for (int i = 0; i < m_nHeight; i++)
    {
        coord.Y = (SHORT)i;
        
        SetConsoleCursorPosition(m_hStdOut, coord);
        
        if (i == (m_nLogHeight - 1))
        {
            char seper[256];

            memset(seper, '=', sizeof(seper));
            WriteConsole(m_hStdOut, seper, m_nWidth, &dwWritten, NULL);
        }
        else
        {
            char empty[256];

            memset(empty, ' ', sizeof(empty));
            WriteConsole(m_hStdOut, empty, m_nWidth, &dwWritten, NULL);
        }
    }

    WriteInput();

    return true;
}

bool Console::Input(const char* info)
{
    assert(info != NULL);

    WriteOut((m_strPrompt + info).c_str(), "");

    if (strlen(info) > 0)
    {
        if (m_nSavePos == -1)
        {
            m_SaveInput.push_back(info);
        }
        else
        {
            m_nSavePos = -1;
        }
    }
            
    ParseInput(info);

    return true;
}

int Console::GetColor(const std::string& color) const
{
    if (color.empty())
    {
        return m_nForeColor;
    }
    
    if (color == "red")
    {
        return FORECOLOR_LIGHTRED;
    }

    if (color == "green")
    {
        return FORECOLOR_LIGHTGREEN;
    }

    if (color == "gray")
    {
        return FORECOLOR_GRAY;
    }

    if (color == "white")
    {
        return FORECOLOR_WHITE;
    }

    if (color == "blue")
    {
        return FORECOLOR_BLUE;
    }

    if (color == "yellow")
    {
        return FORECOLOR_YELLOW;
    }

    return m_nForeColor;
}

int Console::GetCmdRow() const
{
    return m_nHeight - 2;
}

void Console::WriteLine(int line, const char* str, unsigned int color)
{
    assert(str != NULL);

    char data[600] = {0};
    WORD attr[600];
    DWORD dwWritten;
    COORD coord = { 0, (SHORT)line };
    strcpy_s(data, sizeof(data), str);
    
    int len = (int)strlen(data);
    for (int i = len; i < m_nWidth; i++)
    {
        data[i] = ' ';
    }

    if (len <= m_nWidth)
        data[m_nWidth] = '\0';
    
    for (int i = 0; i < len; i++)
    {
        attr[i] = color;
    }
    
    WriteConsoleOutputAttribute(m_hStdOut, attr, len, coord,
        &dwWritten);
    WriteConsoleOutputCharacterA(m_hStdOut, data, (DWORD)strlen(data), coord,
        &dwWritten); 
}

bool Console::WriteLog(const char* str, const char* color)
{
    assert(m_bLoaded);
    assert(m_nLogHeight >= 2);

    char data[512];
    SMALL_RECT rect;
    COORD coord = { 0, 0 };
    CHAR_INFO ciFill;

    // scroll log information
    rect.Left = 0;
    rect.Top = 1;
    rect.Right = m_nWidth - 1;
    rect.Bottom = m_nLogHeight - 2;
    
    ciFill.Char.AsciiChar = ' ';
    ciFill.Attributes = 0;
    
    ScrollConsoleScreenBuffer(m_hStdOut, &rect, NULL, coord, &ciFill);

    SYSTEMTIME tm;

    GetLocalTime(&tm);
    sprintf_s(data, sizeof(data), "[%02d:%02d:%02d.%03d] %s", 
        tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds, str);
    WriteLine(m_nLogHeight - 2, data, GetColor(color));

    return true;
}

bool Console::WriteOut(const char* str, const char* color)
{
    SMALL_RECT rect;
    COORD coord = { 0, (SHORT)m_nLogHeight };
    CHAR_INFO ciFill;

    // scroll output
    rect.Left = 0;
    rect.Top = m_nLogHeight + 1;
    rect.Right = m_nWidth - 1;
    rect.Bottom = GetCmdRow() - 2;
    
    ciFill.Char.AsciiChar = ' ';
    ciFill.Attributes = 0;
    
    ScrollConsoleScreenBuffer(m_hStdOut, &rect, NULL, coord, &ciFill);
    WriteLine(GetCmdRow() - 2, str, GetColor(color));

    return true;
}

bool Console::ParseInput(const char* info)
{
    assert(info != NULL);
    if (m_cb_cmd)
    {
        m_cb_cmd(info);
    }
    return true;
}

NS_YY_END