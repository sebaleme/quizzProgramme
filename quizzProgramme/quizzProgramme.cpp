// quizzProgramme.cpp : Defines the entry point for the application.
// Using the following tutorials:
// https://learn.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp
// https://learn.microsoft.com/en-us/windows/win32/menurc/resources
// http://www.winprog.org/tutorial

#include "framework.h"
#include "quizzProgramme.h"
#include "include\\data_acquisition.hpp"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBITMAP ibiki = NULL;
HDC hdcMem;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Rules(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_QUIZZPROGRAMME, szWindowClass, MAX_LOADSTRING);
    ATOM res = MyRegisterClass(hInstance);

    if (!res)
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Initialize random function
    srand(static_cast<uint32_t>(time(NULL)));
    // Read input data
    init_input_data();

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUIZZPROGRAMME));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)>0)
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUIZZPROGRAMME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+15);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUIZZPROGRAMME);
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

    HMENU hmenu = GetMenu(hWnd);
    MENUITEMINFO menuItemTraining = { 0 };
    MENUITEMINFO menuItemExam = { 0 };

    switch (message)
    {
    // Handling left mouse clicks
    case WM_LBUTTONDOWN:
        {
            char szFileNameTmp[MAX_PATH]; 
            LPWSTR szFileName = LPTSTR(szFileNameTmp); // hopefully it has at least the size [MAX_PATH]
            HINSTANCE hInstance = GetModuleHandle(NULL);

            GetModuleFileName(hInstance, szFileName, MAX_PATH);
            MessageBox(hWnd, szFileName, L"This program is:", MB_OK | MB_ICONINFORMATION);
        }
    // Handling menu option actions
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_RULES:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_RULESBOX), hWnd, Rules);
                break;
            case IDM_MODE_TRAINING:
            case IDM_MODE_EXAM:
                menuItemTraining.cbSize = sizeof(MENUITEMINFO);
                menuItemTraining.fMask = MIIM_STATE;
                menuItemExam.cbSize = sizeof(MENUITEMINFO);
                menuItemExam.fMask = MIIM_STATE;
                GetMenuItemInfo(hmenu, IDM_MODE_TRAINING, FALSE, &menuItemTraining);
                GetMenuItemInfo(hmenu, IDM_MODE_EXAM, FALSE, &menuItemExam);
                if (menuItemTraining.fState == MFS_CHECKED) {
                    // Checked, uncheck it
                    menuItemTraining.fState = MFS_UNCHECKED;
                    menuItemExam.fState = MFS_CHECKED;
                }
                else {
                    // Unchecked, check it
                    menuItemTraining.fState = MFS_CHECKED;
                    menuItemExam.fState = MFS_UNCHECKED;
                }
                SetMenuItemInfo(hmenu, IDM_MODE_TRAINING, FALSE, &menuItemTraining);
                SetMenuItemInfo(hmenu, IDM_MODE_EXAM, FALSE, &menuItemExam);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE: // Called at start
        // Initialize "rules" menu
        ibiki = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_IBIKI));
        if (ibiki == NULL)
            MessageBox(hWnd, L"Could not load IDB_IBIKI!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        // Initialize "Mode" menu
        menuItemTraining.cbSize = sizeof(MENUITEMINFO);
        menuItemTraining.fMask = MIIM_STATE;
        menuItemTraining.fState = MFS_CHECKED;
        SetMenuItemInfo(hmenu, IDM_MODE_TRAINING, FALSE, &menuItemTraining);
        break;
    // Handling window display
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            TCHAR greeting[] = _T("Hello, welcome in this quizz test!");
            TCHAR selectTheme[] = _T("Please select a theme");
            // Here your application is laid out.
            // in the top left corner.
            TextOut(hdc,
                5, 5,
                greeting, _tcslen(greeting));
            TextOut(hdc,
                5, 25,
                selectTheme, _tcslen(selectTheme));
            FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
            //Image* image = Image::FromFile(filePath);
            //Status status = graphic.DrawImage(image, 10, 20);
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
INT_PTR CALLBACK About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK Rules(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    case WM_PAINT:
        BITMAP bm;
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcMem, ibiki));

        GetObject(ibiki, sizeof(bm), &bm);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        EndPaint(hWnd, &ps);
        break;
    }
    return (INT_PTR)FALSE;
}
