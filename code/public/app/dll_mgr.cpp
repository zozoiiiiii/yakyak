#include "dll_mgr.h"
#ifdef _WIN32
#include <Windows.h>
#endif
#include "../external/tinyxml/tinyxml.h"
#include "core/inc/yy_exception.h"
#include "core/inc/yy_file.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <dlfcn.
DllMgr::DllMgr()
{}

DllMgr::~DllMgr()
{}


void DllMgr::Load(YY::IObjectMgr* pEntMgr, const char* file)
{
// 	std::string projectPath = pEntMgr->GetGlobal("ProjectPath").GetStr();
// 	std::string libPath = projectPath + "/lib/x86/libyy_render.so";
// 	void* handle = dlopen(libPath.c_str(), RTLD_LAZY);
// 	if (!handle)
// 		return;
// 
// 
// 	typedef void(*cb_Plugin)(YY::IObjectMgr*);
// 	cb_Plugin func = (cb_Plugin)dlsym(handle, "dll_main");
// 	func(pEntMgr);
// 	dlclose(handle);
    TiXmlDocument doc(file);
    throw_assert(doc.LoadFile(), "load file failed."<<file);

    TiXmlElement* root = doc.RootElement();
    throw_assert(NULL != root, "cannot find root element");

    TiXmlElement* Plugin = root->FirstChildElement("plugin");
    while(NULL != Plugin)
    {
        const char* name = Plugin->Attribute("name");

        std::string path = YY::ExePath() + "\\plugins\\" + name+".dll";
        Insert(pEntMgr, name, path.c_str());

        Plugin = Plugin->NextSiblingElement("plugin");
    }
}

void DllMgr::Insert(YY::IObjectMgr* pEntMgr, const char* name, const char* path)
{

#ifdef _WIN32

    HMODULE hdll = ::LoadLibraryA(path);
    throw_assert(NULL != hdll, "load library"<<path<<" failed.");
    
    //调用dll导出的函数
    typedef void (*cb_Plugin)(YY::IObjectMgr*);
    cb_Plugin func = (cb_Plugin)::GetProcAddress(hdll, "dll_main" );
    if ( NULL == func )
    {
        FreeLibrary(hdll);
        return;
    }

	(*func)(pEntMgr);
#elif __ANDROID__

#elif __APPLE__
#endif
}