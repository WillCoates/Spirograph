// Spirograph.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MainWindow.h"

using namespace Gdiplus;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	// So aparently we need to intialize GDI+ here
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms533895(v=vs.85).aspx
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	MainWindow mainWindow(hInstance);

	mainWindow.Show(nCmdShow);
	mainWindow.Update();

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
		TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	GdiplusShutdown(gdiplusToken);

    return msg.wParam;
}
