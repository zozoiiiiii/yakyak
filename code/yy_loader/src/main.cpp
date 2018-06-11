/************************************************************************/
/*
@author:  junliang
@brief:   main
@time:    20180528
*/
/************************************************************************/
//#include "yy_core.h"
#include <string>
#include "reflection/yy_type.h"
#include "util/inc/yy_time.h"
#include "core/inc/yy_exception.h"
int main()
{
	std::string strTimeStamp = YY::StrTimeStamp();
	throw_assert(false, "hi");
	return 0;
}




/*

#include <iostream>
#include <windows.h>
#include "public/app/app.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    bool ret = App::Instance()->OnMsg(message,(unsigned int) wParam, (unsigned int)lParam);
    if(!ret)
        return DefWindowProc(hWnd, message, wParam, lParam);

    return TRUE;
}



HWND CreateMyWindow(int width, int height, const std::string& title, WNDPROC func)
{
    WNDCLASSEX wndclass = {0}; 
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wndclass.cbSize         = sizeof(WNDCLASSEX);
    wndclass.style         = CS_OWNDC;
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

    //AdjustWindowRect ( &windowRect, wStyle, FALSE );

    HWND hWnd = CreateWindow("WINCLASS1",title.c_str(),WS_VISIBLE | WS_SYSMENU | WS_CAPTION,0,0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    throw_assert( hWnd != NULL, "create windows failed. ");

    ShowWindow ( hWnd, TRUE );
    return hWnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int iCmdShow)
{
    int width=1024;
    int height=768;
    HWND nHwnd = CreateMyWindow(width,height, "hello", &WndProc);
    App::Instance()->Open(width, height, nHwnd);

    MSG	msg;
    for (;;)
	{
		MSG msg;
		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if ( GetMessage (&msg, NULL, 0, 0) )
			{
				TranslateMessage (&msg);
				DispatchMessage (&msg);
			}
		}

        App::Instance()->Excute();
    }
    return 0;
}
*/