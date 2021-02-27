// theme.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "theme.h"
#include <array>
#include <uxtheme.h>
#include <vssym32.h>	// BP_PUSHBUTTON, etc

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AboutTheme(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_THEME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THEME));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THEME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_THEME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_HELP_ABOUTTHEME:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT_THEME), hWnd, AboutTheme);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool DrawMyControl(HDC hDC, HWND hwndButton, int iState)
{
	HTHEME hTheme = OpenThemeData(hwndButton, _T("Button"));
	if (!hTheme)
		return false;

	RECT rc, rcContent;
	TCHAR szButtonText[255];
	HRESULT hr;
	size_t cch;

	GetWindowRect(hwndButton, &rc);
	GetWindowText(hwndButton, szButtonText, std::size(szButtonText));
	cch = _tcslen(szButtonText);
	if (hTheme)
	{
		hr = DrawThemeBackground(hTheme, hDC, BP_PUSHBUTTON, iState, &rc, 0);
		// Always check your result codes.
		hr = GetThemeBackgroundContentRect(hTheme, hDC, BP_PUSHBUTTON, iState, &rc, &rcContent);
		hr = DrawThemeText(hTheme, hDC, BP_PUSHBUTTON, iState, szButtonText, cch, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, &rcContent);
		CloseThemeData(hTheme);
		return true;
	}
	return false;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for theme box.
INT_PTR CALLBACK AboutTheme(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			HWND hStatic = GetDlgItem(hDlg, IDC_STATIC);
			BOOL isThemed = IsAppThemed();
			WCHAR themeFileName[MAX_PATH] = {};
			WCHAR colorBuff[MAX_PATH] = {};
			WCHAR sizeBuff[MAX_PATH] = {};
			if (isThemed)
			{
				HRESULT hr = GetCurrentThemeName(themeFileName, std::size(themeFileName), colorBuff, std::size(colorBuff), sizeBuff, std::size(sizeBuff));
			}
			TCHAR windowText[MAX_PATH] = {};
			_stprintf_s(windowText, _T(" isThemed: %d \n themeFileName: %s \n colorBuff: %s \n sizeBuff: %s"),
				isThemed, themeFileName, colorBuff, sizeBuff);
			if (hStatic)
				SetWindowText(hStatic, windowText);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	case WM_PAINT:
		{
			HDC hdc = GetDC(hDlg);
			HWND hButton = GetDlgItem(hDlg, IDOK);
			if (hButton && hdc)
				DrawMyControl(hdc, hButton, PBS_NORMAL);
			if (hdc)
				ReleaseDC(hDlg, hdc);
			return TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}