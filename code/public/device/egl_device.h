/************************************************************************/
/* 
@author:    junliang
@brief:     native window system(windows, android, ios) | egl | opengl es

egl create one surface -> opengl es render on this surface.

@time:      5/26/2016
*/
/************************************************************************/
#pragma once

#include <EGL/egl.h>
#include <string>

class EGLDevice
{
public:
    EGLDevice():m_bInited(false){}
    virtual ~EGLDevice(){}

    void InitDevice(EGLNativeWindowType handle);
    void SwapBuffers();
	void GetConfigAttrib(EGLint attribute, EGLint *value);
	void ResetSurface();
	EGLint getFormat() { return m_format; }
private:
    void CreateEGL(EGLNativeWindowType handle);
private:
	bool m_bInited;
    EGLNativeWindowType m_handle;


    /// EGL display
    EGLDisplay  m_display;

    /// EGL context
    EGLContext  m_context;

    /// EGL surface
    EGLSurface  m_surface;

	EGLConfig m_config;
	EGLint          m_format;
};
