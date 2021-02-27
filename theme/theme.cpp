// theme.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "theme.h"
#include <array>
#include <uxtheme.h>
#include <vssym32.h>	// BP_PUSHBUTTON, etc
#include <map>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

std::map<int, COLORREF> g_themeColors;

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

bool DrawMyButton(HDC hDC, RECT rcPaint, HWND hwnd)
{
	int state = SendMessage(hwnd, BM_GETSTATE, 0, 0);
	int partState = 0;
	if (state & BST_FOCUS)
		partState |= PBS_DEFAULTED;
	if (state & BST_HOT)
		partState |= PBS_HOT;
	if (state & BST_PUSHED)
		partState |= PBS_PRESSED;
	if (partState == 0)
		partState = PBS_NORMAL;
	if (!IsWindowEnabled(hwnd))
		partState |= PBS_DISABLED;

	HRESULT hr = S_OK;
	HTHEME hTheme = OpenThemeData(hwnd, L"button");
	if (hTheme)
	{
		hr = DrawThemeParentBackground(hwnd, hDC, &rcPaint);

		hr = DrawThemeBackground(hTheme, hDC, BP_PUSHBUTTON, partState, &rcPaint, NULL);

		hr = DrawThemeEdge(hTheme, hDC, BP_PUSHBUTTON, partState,
			&rcPaint, EDGE_RAISED, BF_RECT | ((state & BST_PUSHED) ? BF_FLAT : 0), NULL);

		DTTOPTS opts = {};
		opts.dwSize = sizeof(opts);
		opts.crText = g_themeColors[COLOR_BTNTEXT];
		opts.dwFlags |= DTT_TEXTCOLOR;
		WCHAR caption[MAX_PATH];
		GetWindowText(hwnd, caption, std::size(caption));
		size_t cch = _tcslen(caption);
		hr = DrawThemeTextEx(hTheme, hDC, BP_PUSHBUTTON, partState,
			caption, cch, DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			&rcPaint, &opts);

		CloseThemeData(hTheme);
		return true;
	}
	else
	{
		// Draw the control without using visual styles.
	}
	return false;
}

LRESULT CALLBACK SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// All painting occurs here, between BeginPaint and EndPaint.
			DrawMyButton(hdc, ps.rcPaint, hWnd);
			//HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
			//FillRect(hdc, &ps.rcPaint, hBlackBrush);
			EndPaint(hWnd, &ps);
		}
		return TRUE;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
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

#define BTN_SUBCLASS_ID 111

std::map<int, COLORREF> GetThemeColors(HTHEME hTheme)
{
	static const int colorIds[] = {
		COLOR_SCROLLBAR,
		COLOR_BACKGROUND,
		COLOR_ACTIVECAPTION,
		COLOR_INACTIVECAPTION,
		COLOR_MENU,
		COLOR_WINDOW,
		COLOR_WINDOWFRAME,
		COLOR_MENUTEXT,
		COLOR_WINDOWTEXT,
		COLOR_CAPTIONTEXT,
		COLOR_ACTIVEBORDER,
		COLOR_INACTIVEBORDER,
		COLOR_APPWORKSPACE,
		COLOR_HIGHLIGHT,
		COLOR_HIGHLIGHTTEXT,
		COLOR_BTNFACE,
		COLOR_BTNSHADOW,
		COLOR_GRAYTEXT,
		COLOR_BTNTEXT,
		COLOR_INACTIVECAPTIONTEXT,
		COLOR_BTNHIGHLIGHT,
#if(WINVER >= 0x0400)
		COLOR_3DDKSHADOW,
		COLOR_3DLIGHT,
		COLOR_INFOTEXT,
		COLOR_INFOBK,
#endif

#if(WINVER >= 0x0500)
		COLOR_HOTLIGHT,
		COLOR_GRADIENTACTIVECAPTION,
		COLOR_GRADIENTINACTIVECAPTION,
#if(WINVER >= 0x0501)
		COLOR_MENUHILIGHT,
		COLOR_MENUBAR,
#endif 
#endif 
#if(WINVER >= 0x0400)
		COLOR_DESKTOP,
		COLOR_3DFACE,
		COLOR_3DSHADOW,
		COLOR_3DHIGHLIGHT,
		COLOR_3DHILIGHT,
		COLOR_BTNHILIGHT
#endif
	};
	std::map<int, COLORREF> result;
	for (int colorId : colorIds)
	{
		COLORREF color = GetThemeSysColor(hTheme, colorId);
		result[colorId] = color;
	}
	return result;
}

std::map<int, COLORREF> GetDarkThemeColors(HWND hParent)
{
	std::map<int, COLORREF> result;
	HWND htmp = CreateWindow(L"button", L"", WS_CHILDWINDOW, 0, 0, 0, 0, hParent, NULL, NULL, NULL);
	if (htmp)
	{
		HRESULT hr = S_OK;
		hr = SetWindowTheme(htmp, L"DarkMode_Explorer", NULL);
		if (SUCCEEDED(hr))
		{
			if (HTHEME hTheme = OpenThemeData(htmp, L"button"))
			{
				result = GetThemeColors(hTheme);
				CloseThemeData(hTheme);
			}
		}
		DestroyWindow(htmp);
	}
	return result;
}

// Message handler for theme box.
INT_PTR CALLBACK AboutTheme(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			//// TODO: Disable visual styles
			//SetWindowTheme(hDlg, L" ", L" ");
			g_themeColors = GetDarkThemeColors(hDlg);
			HWND hButton = GetDlgItem(hDlg, IDOK);
			// Get theme info
			HWND hStatic = GetDlgItem(hDlg, IDC_STATIC);
			BOOL isThemed = IsAppThemed();
			BOOL isThemeActive = IsThemeActive();
			WCHAR themeFileName[MAX_PATH] = {};
			WCHAR colorBuff[MAX_PATH] = {};
			WCHAR sizeBuff[MAX_PATH] = {};
			if (isThemed)
			{
				HRESULT hr = GetCurrentThemeName(themeFileName, std::size(themeFileName), colorBuff, std::size(colorBuff), sizeBuff, std::size(sizeBuff));
			}

			TCHAR windowText[1024] = {};
			_stprintf_s(windowText,
				_T("Is Theme Active (classic look and feel is off): %d\n")
				_T("Is App Themed (classic + manifest + compatibility): %d\n")
				_T("Theme File: %s\n")
				_T("Color: %s, Size: %s\n")
				_T("Theme Sys Color (COLOR_BTNFACE): %x\n"),
				isThemed, isThemeActive, themeFileName, colorBuff, sizeBuff, g_themeColors[COLOR_BTNFACE]);
			if (hStatic)
				SetWindowText(hStatic, windowText);
			// Subclass
			SetWindowSubclass(hButton, &SubclassProc, BTN_SUBCLASS_ID, 0);
		}
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