#include "egl_device.h"
#include "yy_core.h"

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8


#define WINDOW_CLASS_NAME "WINCLASS1"


void EGLDevice::CreateEGL(EGLNativeWindowType handle)
{
	// android test
	EGLint attribList[] =
	{
		EGL_LEVEL,          0,
		EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,  EGL_FALSE,
		EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,

		EGL_SAMPLE_BUFFERS,  0,                 // close sample buffer by default
		EGL_SAMPLES,        0,

		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_ALPHA_SIZE,     8,    // alhpa

		EGL_DEPTH_SIZE, 8,
		EGL_STENCIL_SIZE, 8,
		//EGL_BUFFER_SIZE,    EGL_BLUE_SIZE+EGL_GREEN_SIZE+EGL_RED_SIZE+EGL_ALPHA_SIZE
		//EGL_STENCIL_SIZE,   4,
		EGL_NONE
	};

        EGLint numConfigs;
        EGLint majorVersion;
        EGLint minorVersion;
        EGLDisplay display;
        EGLContext context;
        EGLSurface surface;
        //EGLConfig config;
        EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

		//EGLNativeDisplayType display_id;
#ifdef _WIN32

		// EGLNativeDisplayType equals to  HDC
		//display_id = ::GetDC(m_handle);
#elif __ANDROID__
		// EGLNativeDisplayType equals to ANativeWindow *
#elif __APPLE__
#endif

        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if ( display == EGL_NO_DISPLAY )
		{
			// Unable to open connection to local windowing system
            return;
        }

        m_display=display;

        // 2,Initialize EGL
        if ( !eglInitialize(display, &majorVersion, &minorVersion) )
		{
			// Unable to initialize EGL. Handle and recover
            return;
        }

        // 3, Get configs
        if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
        {
            return;
        }

        // Choose config
        if ( !eglChooseConfig(display, attribList, &m_config, 1, &numConfigs) )
        {
            return;
        }


#ifdef __ANDROID__
		eglGetConfigAttrib(display, m_config, EGL_NATIVE_VISUAL_ID, &m_format);
		ANativeWindow_setBuffersGeometry(handle, 0, 0, m_format);
#endif


        // 4, Create a surface, eglCreatePbufferSurface can create offline surface, is instead by framebuffer, which can controlled by opengl es
        surface = eglCreateWindowSurface(display, m_config, handle, NULL);
        if ( surface == EGL_NO_SURFACE )
        {
            return;
        }

        m_surface=surface;

        // 5, Create a GL context
        context = eglCreateContext(display, m_config, EGL_NO_CONTEXT, contextAttribs );
        if ( context == EGL_NO_CONTEXT )
        {
            return;
        }

        m_context=context;

        // 6, Make the context current
        if ( !eglMakeCurrent(display, surface, surface, context) )
        {
            return;
        }
}


void EGLDevice::InitDevice(EGLNativeWindowType handle)
{
    m_handle = handle;
    CreateEGL(m_handle);
	m_bInited = true;
}

void EGLDevice::SwapBuffers()
{
	if(m_bInited)
		eglSwapBuffers ( m_display, m_surface );
}


void EGLDevice::GetConfigAttrib(EGLint attribute, EGLint *value)
{
	eglGetConfigAttrib(m_display, m_config, attribute, value);
}

void EGLDevice::ResetSurface()
{
	if (m_surface != EGL_NO_SURFACE)
	{
		eglDestroySurface(m_display, m_surface);
	}

	EGLint surfaceAttrs[] =
	{
		EGL_RENDER_BUFFER,
		EGL_BACK_BUFFER,
		EGL_NONE
	};

	m_surface = eglCreateWindowSurface(m_display, m_config, m_handle, surfaceAttrs);

	if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == false)
	{
		//ALog_A(false, "AEGLTool ResetSurface failed EGL unable to eglMakeCurrent");
	}
}