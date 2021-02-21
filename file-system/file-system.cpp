// file-system.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "file-system.h"
#include <shlwapi.h>
#include <array>    // size

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
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILESYSTEM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FILESYSTEM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

// Message handler for "Check Path" dialog.
// TODO: use a list box?
INT_PTR CALLBACK CheckPathDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    static TCHAR buffer[MAX_PATH] = {};
    static TCHAR buffer2[MAX_PATH] = {};
    static TCHAR path[MAX_PATH] = {};
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            HWND hChild = GetDlgItem(hDlg, IDC_PATH_EDIT);
            if (hChild)
                GetWindowText(hChild, path, std::size(path));
            using PathQuery = BOOL(__stdcall *)(LPCTSTR);
            struct PathQueryPack
            {
                int id;
                const TCHAR* name;
                PathQuery func;
            };
            PathQueryPack pathQueries[] = {
                {IDC_STATIC_ISDIR, _T("PathIsDirectory"), &PathIsDirectory},
                {IDC_STATIC_FILE_EXISTS, _T("PathFileExists"), &PathFileExists},
                {IDC_STATIC_IS_NET_PATH, _T("PathIsNetworkPath"), &PathIsNetworkPath},
                {IDC_STATIC_IS_REL, _T("PathIsRelative"), &PathIsRelative},
                {IDC_STATIC_IS_ROOT, _T("PathIsRoot"), &PathIsRoot},
                {IDC_STATIC_IS_UNC, _T("PathIsUNC"), &PathIsUNC},
                {IDC_STATIC_IS_URL, _T("PathIsURL"), &PathIsURL},
            };
            for (auto&& query : pathQueries)
            {
                HWND hChild = GetDlgItem(hDlg, query.id);
                if (hChild)
                {
                    _stprintf_s(buffer, _T("%s: %d"), query.name, query.func(path));
                    SetWindowText(hChild, buffer);
                }
            }
            hChild = GetDlgItem(hDlg, IDC_STATIC_FULL_PATH);
            if (hChild)
            {
                TCHAR** lppPart = { NULL };
				if (GetFullPathName(path, std::size(path), buffer2, lppPart) != 0)
                {
                    _stprintf_s(buffer, _T("GetFullPathName: %s"), buffer2);
                }
                else
                {
                    _stprintf_s(buffer, _T("GetFullPathName: ?"));
                }
            }
            hChild = GetDlgItem(hDlg, IDC_STATIC_PATH_CANON);
            if (hChild)
            {
                if (PathCanonicalize(buffer2, path))
                {
                    _stprintf_s(buffer, _T("PathCanonicalize: %s"), buffer2);
                }
                else
                {
                    _stprintf_s(buffer, _T("PathCanonicalize: ?"));
                }
                SetWindowText(hChild, buffer);
            }
            hChild = GetDlgItem(hDlg, IDC_STATIC_CURR_DIR);
            if (hChild)
            {
                if (GetCurrentDirectory(std::size(path), path) != 0)
                {
                    _stprintf_s(buffer, _T("GetCurrentDirectory: %s"), path);
                }
                else
                {
                    _stprintf_s(buffer, _T("GetCurrentDirectory: ?"));
                }
                SetWindowText(hChild, buffer);
            }
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

