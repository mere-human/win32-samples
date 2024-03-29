// main.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "common.h"
#include <shlwapi.h>
#include <array>    // size
#include <windowsx.h>   // ListBox

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
INT_PTR CALLBACK    CheckPathDlgProc(HWND, UINT, WPARAM, LPARAM);

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
	LoadStringW(hInstance, IDC_FILESYSTEM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILESYSTEM));

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

void ShowDateTime(HWND hWnd)
{
	const TCHAR* title = _T("Date & Time");
	const TCHAR* fmt = _T("yyyy-MM-dd hh:mm:ss");
	const TCHAR* localeName = _T("en_US");
	const DWORD flags = 0;

	SYSTEMTIME dateTime = {};
	GetLocalTime(&dateTime);
	TCHAR buffer[MAX_PATH] = {};
	if (GetDateFormatEx(localeName, flags, &dateTime, fmt, buffer, (int)std::size(buffer), NULL) != 0)
	{
		if (GetTimeFormatEx(localeName, flags, &dateTime, buffer, buffer, (int)std::size(buffer)))
			MessageBox(hWnd, buffer, title, MB_OK);
		else
			MessageBox(hWnd, _T("Error: GetTimeFormatEx"), title, MB_OK | MB_ICONERROR);
	}
	else
	{
		MessageBox(hWnd, _T("Error: GetDateFormatEx"), title, MB_OK | MB_ICONERROR);
	}
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILESYSTEM));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FILESYSTEM);
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
		case IDM_FILE_PATH:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PATH_DIALOG), hWnd, CheckPathDlgProc);
			break;
		case IDM_DATETIME:
			ShowDateTime(hWnd);
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

void FillListBox(HWND hDlg)
{
	TCHAR path[MAX_PATH] = {};

	HWND hEdit = GetDlgItem(hDlg, IDC_PATH_EDIT);
	if (hEdit)
		GetWindowText(hEdit, path, std::size(path));
	if (_tcslen(path) == 0)
	{
		if (GetModuleFileName(NULL, path, std::size(path)) != 0)
			SetWindowText(hEdit, path);
	}

	HWND hList = GetDlgItem(hDlg, IDC_LIST_PATH_INFO);
	ListBox_ResetContent(hList);

	TCHAR buffer[MAX_PATH] = {};
	TCHAR buffer2[MAX_PATH] = {};

	using PathQuery = BOOL(__stdcall*)(LPCTSTR);
	struct PathQueryPack
	{
		const TCHAR* name;
		PathQuery func;
	};
	PathQueryPack pathQueries[] = {
		{_T("PathIsDirectory"), &PathIsDirectory},
		{_T("PathFileExists"), &PathFileExists},
		{_T("PathIsNetworkPath"), &PathIsNetworkPath},
		{_T("PathIsRelative"), &PathIsRelative},
		{_T("PathIsRoot"), &PathIsRoot},
		{_T("PathIsUNC"), &PathIsUNC},
		{_T("PathIsURL"), &PathIsURL},
	};

	for (auto&& query : pathQueries)
	{
		_stprintf_s(buffer, _T("%s: %d"), query.name, query.func(path));
		ListBox_AddString(hList, buffer);
	}

	TCHAR** lppPart = { NULL };
	if (GetFullPathName(path, std::size(path), buffer2, lppPart) != 0)
		_stprintf_s(buffer, _T("GetFullPathName: %s"), buffer2);
	else
		_stprintf_s(buffer, _T("GetFullPathName: ?"));
	ListBox_AddString(hList, buffer);

	if (PathCanonicalize(buffer2, path))
		_stprintf_s(buffer, _T("PathCanonicalize: %s"), buffer2);
	else
		_stprintf_s(buffer, _T("PathCanonicalize: ?"));
	ListBox_AddString(hList, buffer);

	_tcsncpy_s(buffer2, path, std::size(path));
	if (PathRemoveFileSpec(buffer2))
		_stprintf_s(buffer, _T("PathRemoveFileSpec: %s"), buffer2);
	else
		_stprintf_s(buffer, _T("PathRemoveFileSpec: ?"));
	ListBox_AddString(hList, buffer);

	if (GetCurrentDirectory(std::size(buffer2), buffer2) != 0)
		_stprintf_s(buffer, _T("GetCurrentDirectory: %s"), buffer2);
	else
		_stprintf_s(buffer, _T("GetCurrentDirectory: ?"));
	ListBox_AddString(hList, buffer);
}

// Message handler for "Check Path" dialog.
INT_PTR CALLBACK CheckPathDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		FillListBox(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			FillListBox(hDlg);
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

