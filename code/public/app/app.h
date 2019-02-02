/************************************************************************/
/* 
@author:    junliang
@brief:     application
@time:      2017-7-28
*/
/************************************************************************/
#pragma once


#include "yy_core.h"
//#include <Windows.h>

//class YY::IObjectMgr;
class App
{
public:
	App();
    ~App(){}
    static App* Instance();

/*#if defined(_WIN32) || defined(__VC32__) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__) /* Win32 and WinCE */  
//    typedef HWND    EGLNativeWindowType;
//#elif defined(__WINSCW__) || defined(__SYMBIAN32__)  /* Symbian */  
  //  typedef void *EGLNativeWindowType;
//#elif defined(__ANDROID__) || defined(ANDROID)  
  //  typedef struct ANativeWindow*           EGLNativeWindowType;
//#elif defined(__unix__)  
  //  typedef Window   EGLNativeWindowType;

    void Open(float width, float height, const std::string& resPath="" );
    void Close();
    void Excute();

	void Pause() {}
	void Resume() {}
	void Resize(float width, float height);
    bool IsOpened(){return m_bOpened;}

    // if want default msg proc, return false.
    bool OnMsg(unsigned int msg, size_t param1, size_t param2);

	void setPath(const std::string& path) { m_pEntMgr->SetGlobal("ProjectPath", path); }
	std::string getPath() { return m_pEntMgr->FindGlobal("ProjectPath").GetStr(); }
private:
    YY::IObjectMgr* m_pEntMgr;
    bool m_bOpened;
};