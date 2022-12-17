// quizzProgramme.cpp : Defines the entry point for the application.

#include "..\\include\\framework.h"
#include "..\\include\\quizzProgramme.h"
#include "..\\include\\data_acquisition.hpp"
#include "..\\include\\test_mode.hpp"
#include "..\\include\\training_mode.hpp"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBITMAP ibiki = NULL;
HDC hdcMem;
themes s_selection;
TCHAR question[256];
std::unique_ptr<CCurrentSession> s_pCurrentSession{ nullptr };
string s_question;

// Initialize variables for controls
HFONT hfTextBox, hfButtonBox, hfAnswerTextBox, hfAnswerButtonBox;
HWND hEditTextBox, hEditButtonBox, hAnswerEditTextBox, hAnswerEditButtonBox;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Rules(HWND, UINT, WPARAM, LPARAM);

void init(HMENU& f_hmenu, themes f_theme)
{
    s_pCurrentSession->setTheme(f_theme);
    init_input_data(f_theme);
    // Initialize "Theme" menu
    MENUITEMINFO menuItemNaruto = { 0 };
    MENUITEMINFO menuItemGot = { 0 };
    menuItemNaruto.cbSize = sizeof(MENUITEMINFO);
    menuItemNaruto.fMask = MIIM_STATE;
    menuItemGot.cbSize = sizeof(MENUITEMINFO);
    menuItemGot.fMask = MIIM_STATE;
    if (f_theme == themes::Naruto)
    {
        menuItemGot.fState = MFS_UNCHECKED;
        menuItemNaruto.fState = MFS_CHECKED;
    }
    else if (f_theme == themes::GoT)
    {
        menuItemGot.fState = MFS_CHECKED;
        menuItemNaruto.fState = MFS_UNCHECKED;
    }

    SetMenuItemInfo(f_hmenu, IDM_QUIZZTYPE_NARUTO, FALSE, &menuItemNaruto);
    SetMenuItemInfo(f_hmenu, IDM_QUIZZTYPE_GOT, FALSE, &menuItemGot);
}

void start()
{
    if (s_pCurrentSession->getMode() == quizz_mode::TEST)
    {
        test_mode(*s_pCurrentSession);
    }
    else // quizz_mode::TRAINING
    {
    }
}

void toggleMode(HMENU& f_hmenu, MENUITEMINFO& f_menuItemTraining)
{
    MENUITEMINFO menuItemExam = { 0 };
    f_menuItemTraining.cbSize = sizeof(MENUITEMINFO);
    f_menuItemTraining.fMask = MIIM_STATE;
    menuItemExam.cbSize = sizeof(MENUITEMINFO);
    menuItemExam.fMask = MIIM_STATE;
    GetMenuItemInfo(f_hmenu, IDM_MODE_TRAINING, FALSE, &f_menuItemTraining);
    GetMenuItemInfo(f_hmenu, IDM_MODE_EXAM, FALSE, &menuItemExam);
    if (f_menuItemTraining.fState == MFS_CHECKED) {
        // Checked, uncheck it
        f_menuItemTraining.fState = MFS_UNCHECKED;
        menuItemExam.fState = MFS_CHECKED;
    }
    else {
        // Unchecked, check it
        f_menuItemTraining.fState = MFS_CHECKED;
        menuItemExam.fState = MFS_UNCHECKED;
    }
    SetMenuItemInfo(f_hmenu, IDM_MODE_TRAINING, FALSE, &f_menuItemTraining);
    SetMenuItemInfo(f_hmenu, IDM_MODE_EXAM, FALSE, &menuItemExam);
}


// Creating new instance of a window
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

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
    MENUITEMINFO menuItemTraining = { 0 }; // Needed outside toggleMode() because used in WM_CREATE

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
                case IDM_QUIZZTYPE_NARUTO:
                    // Read input data
                    init(hmenu, themes::Naruto);
                    break;
                case IDM_QUIZZTYPE_GOT:
                    // Read input data
                    init(hmenu, themes::GoT);
                    break;
                case IDM_MODE_TRAINING:
                    s_pCurrentSession->setMode(quizz_mode::TRAINING);
                    toggleMode(hmenu, menuItemTraining);
                    break;
                case IDM_MODE_EXAM:
                    s_pCurrentSession->setMode(quizz_mode::TEST);
                    toggleMode(hmenu, menuItemTraining);
                    break;
                case IDC_MAIN_BUTTON:
                    s_pCurrentSession->m_gameStarted = true;
                    UpdateWindow(hWnd);
                    ShowWindow(hEditTextBox, SW_HIDE);
                    ShowWindow(hEditButtonBox, SW_HIDE);
                    ShowWindow(hAnswerEditTextBox, SW_SHOW);
                    ShowWindow(hAnswerEditButtonBox, SW_SHOW);
                    InvalidateRect(hWnd, NULL, TRUE);
                    start();
                    break;
                case IDC_ANSWER_BUTTON:
                    TCHAR answer[128];
                    GetDlgItemText(hWnd, IDC_ANSWER_EDIT, answer, 128);
                    training_mode_evaluate(s_pCurrentSession->getTheme(), answer);
                    training_mode_answer(s_pCurrentSession->getTheme());
                    InvalidateRect(hWnd, NULL, TRUE);
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
            s_pCurrentSession = make_unique<CCurrentSession>();
            // Initialize "rules" menu
            ibiki = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_IBIKI));
            if (ibiki == NULL)
                MessageBox(hWnd, L"Could not load IDB_IBIKI!", L"Error", MB_OK | MB_ICONEXCLAMATION);
            // Initialize "Mode" menu
            menuItemTraining.cbSize = sizeof(MENUITEMINFO);
            menuItemTraining.fMask = MIIM_STATE;
            menuItemTraining.fState = MFS_CHECKED;
            SetMenuItemInfo(hmenu, IDM_MODE_TRAINING, FALSE, &menuItemTraining);
            // Initialize "Theme" menu
            init(hmenu, themes::Naruto);

            // Create username input text box
            hEditTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"User",
                WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                15, 80, 85, 20, hWnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
            if (hEditTextBox == NULL)
                MessageBox(hWnd, _T("Could not create edit box."), L"Error", MB_OK | MB_ICONERROR);
            hfTextBox = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hEditTextBox, WM_SETFONT, (WPARAM)hfTextBox, MAKELPARAM(FALSE, 0));

            // Create answer input text box hfAnswerTextBox, hfAnswerButtonBox;
            hAnswerEditTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                15, 200, 250, 20, hWnd, (HMENU)IDC_ANSWER_EDIT, GetModuleHandle(NULL), NULL);
            if (hAnswerEditTextBox == NULL)
                MessageBox(hWnd, _T("Could not create edit box."), L"Error", MB_OK | MB_ICONERROR);
            hfAnswerTextBox = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hAnswerEditTextBox, WM_SETFONT, (WPARAM)hfAnswerTextBox, MAKELPARAM(FALSE, 0));

            // Create start Button box
            hEditButtonBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Start game",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                105, 75, 85, 30, hWnd, (HMENU)IDC_MAIN_BUTTON, GetModuleHandle(NULL), NULL);
            if (hEditButtonBox == NULL)
                MessageBox(hWnd, _T("Could not create edit box."), L"Error", MB_OK | MB_ICONERROR);
            hfButtonBox = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hEditButtonBox, WM_SETFONT, (WPARAM)hfButtonBox, MAKELPARAM(FALSE, 0));

            // Create answer Button box
            hAnswerEditButtonBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Send",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                270, 200, 85, 30, hWnd, (HMENU)IDC_ANSWER_BUTTON, GetModuleHandle(NULL), NULL);
            if (hAnswerEditButtonBox == NULL)
                MessageBox(hWnd, _T("Could not create edit box."), L"Error", MB_OK | MB_ICONERROR);
            hfAnswerButtonBox = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hAnswerEditButtonBox, WM_SETFONT, (WPARAM)hfAnswerButtonBox, MAKELPARAM(FALSE, 0));

            ShowWindow(hEditTextBox, SW_SHOW);
            ShowWindow(hEditButtonBox, SW_SHOW);
            ShowWindow(hAnswerEditTextBox, SW_HIDE);
            ShowWindow(hAnswerEditButtonBox, SW_HIDE);
            break;
        // Handling window display
        case WM_PAINT:
            {
                if (!s_pCurrentSession->m_gameStarted)
                {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hWnd, &ps);
                    RECT rc;
                    TCHAR greeting[] = _T("Hello, welcome in this quizz test!");
                    TCHAR selectTheme[] = _T("Please select a theme, a game type and enter your name");
                    // Here your application is laid out.
                    // in the top left corner.
                    TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
                    TextOut(hdc, 5, 25, selectTheme, _tcslen(selectTheme));

                    FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
                    EndPaint(hWnd, &ps);
                }
                else
                {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hWnd, &ps);
                    RECT rc;
                    TCHAR startQuizz[] = _T("Starting Quizz!");
                    TextOut(hdc, 5, 100, startQuizz, _tcslen(startQuizz));
                    // Print question
                    s_question = training_mode_question(s_pCurrentSession->getTheme(), hWnd);
                    TextOutA(hdc, 25, 120, s_question.c_str(), 30);

                    FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW+5));
                    UpdateWindow(hWnd);
                    EndPaint(hWnd, &ps);
                }
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
