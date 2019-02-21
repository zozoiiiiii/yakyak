/************************************************************************/
/*
@author:  junliang
@brief:   main
@time:    20180528
*/
/************************************************************************/
#include "yy_core.h"
#include <iostream>
#include <windows.h>
#include "public/app/app.h"
#include "public/device/egl_device.h"
#include "algorithm/algorithm.h"

EGLDevice g_eglDevice;
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (App::Instance()->IsOpened())
	{
		if (message == WM_SIZE)
		{
			g_eglDevice.ResetSurface();

			int fwSizeType = wParam; // resizing flag
			int nWidth = LOWORD(lParam); // width of client area
			int nHeight = HIWORD(lParam); // height of client area
			App::Instance()->Resize(nWidth, nHeight);
		}

		bool ret = App::Instance()->OnMsg(message, (unsigned int)wParam, (unsigned int)lParam);
		if (ret)
			return 0;
	}

	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



HWND CreateMyWindow(HINSTANCE hInstance, int width, int height, const std::string& title, WNDPROC func)
{
    WNDCLASSEX wndclass = {0}; 
    RECT     windowRect;
    //HINSTANCE hInstance = GetModuleHandle(NULL);

    wndclass.cbSize         = sizeof(WNDCLASSEX);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;;
    wndclass.lpfnWndProc   = (WNDPROC)func; 
    wndclass.cbClsExtra        = 0;
    wndclass.cbWndExtra        = 0;
    wndclass.hInstance        = hInstance;
    wndclass.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground    = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName    = "WINCLASS1";
    wndclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

    throw_assert( RegisterClassEx (&wndclass) != 0, "create window failed.");


    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = width;
    windowRect.bottom = height;

	DWORD wsStyle = WS_VISIBLE | WS_SYSMENU | WS_CAPTION;
    AdjustWindowRect ( &windowRect, WS_SYSMENU | WS_CAPTION, FALSE );

    HWND hWnd = CreateWindow("WINCLASS1",title.c_str(), wsStyle,0,0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    throw_assert( hWnd != NULL, "create windows failed. ");

    ShowWindow ( hWnd, SW_SHOW);
	UpdateWindow(hWnd);
    return hWnd;
}


bool checkNumber(int* nums, int length)
{
	if (length % 2 == 0)
		return false;

	int midIndex = length / 2;
	for (int i = midIndex+1; i < length; i++)
	{
		int gap = i - midIndex;
		if (nums[i] != nums[midIndex - gap])
			return false;
	}
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int iCmdShow)
{
    int width=1024;
    int height=768;
    HWND nHwnd = CreateMyWindow(hInstance, width,height, "hello", &WndProc);

	g_eglDevice.InitDevice(nHwnd);

	std::string exe_path = YY::ExePath();
	std::string pre_exe_path = YY::GetPrePath(exe_path);
	std::string res = pre_exe_path + "\\res\\";
    App::Instance()->Open(width, height, res);

    MSG	msg;
    for (;;)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return 1;
			}
			
			LOG_TRACE(msg.message);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

        App::Instance()->Excute();

		// in the past, one framebuffer show to one display, use the raster-scan, this would result tearing, in which the sceen shows parts of the old frame and the parts of the new frame;
		// now, there are two framebuffers, the display reads from the front buffer, while we can write the next frame to back buffer. when we finish, we signal to GPU to swap the front and back buffer.
		g_eglDevice.SwapBuffers();
    }
    return 0;
}