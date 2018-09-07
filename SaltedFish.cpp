// SaltedFish.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowProc.h"
#include "resource.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// TODO: Place code here.
	WNDCLASS wc = {0};
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(hInstance, (LPSTR)IDC_CURSOR1);
	wc.hIcon = LoadIcon(hInstance, (LPSTR)IDI_ICON2);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = "MainFrame";
	wc.lpszMenuName = "IDR_MENU1";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	
	if (!RegisterClass(&wc))
	{
		return -1;
	}
	
	HWND hWnd = CreateWindow("MainFrame", "真-拯救咸鱼2之无限滑稽", WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 400, 100, 850, 650, NULL, NULL, hInstance, NULL);
	
	if (NULL == hWnd)
	{
		return -1;
	}
	
	ShowWindow(hWnd, SW_SHOW);
	
	MSG msg = {0};
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}




