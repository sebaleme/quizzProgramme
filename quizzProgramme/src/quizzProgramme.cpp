// quizzProgramme.cpp : Defines the entry point for the application.

#include "..\\include\\framework.h"
#include "..\\include\\quizzProgramme.h"
#include "..\\include\\data_acquisition.hpp"
#include "..\\include\\test_mode.hpp"
#include "..\\include\\training_mode.hpp"
#include <string> 

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBITMAP ibiki = NULL, shadowImage = NULL, worldMapImage = NULL;

HDC hdcMem;
themes s_selection;
std::unique_ptr<CCurrentSession> s_pCurrentSession{ nullptr };
string s_question;

// Initialize variables for controls
HFONT hfTextBox, hfButtonBox, hfStopButtonBox, hfAnswerTextBox, hfAnswerButtonBox;
HWND hEditTextBox, hEditButtonBox, hEditStopButtonBox, hAnswerEditTextBox, hAnswerEditButtonBox;
HWND hWndPictureBox;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Rules(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Hints(HWND, UINT, WPARAM, LPARAM);

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

void stop(HWND f_hWnd)
{
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - s_pCurrentSession->getStart();
    s_pCurrentSession->set_duration(static_cast<int>(elapsed_seconds.count()));
    s_pCurrentSession->m_gameStarted = EGameState::finished;
    UpdateWindow(f_hWnd);
    ShowWindow(hEditTextBox, SW_SHOW);
    ShowWindow(hEditButtonBox, SW_SHOW);
    ShowWindow(hEditStopButtonBox, SW_HIDE);
    ShowWindow(hAnswerEditTextBox, SW_HIDE);
    ShowWindow(hAnswerEditButtonBox, SW_HIDE);
    ShowWindow(hWndPictureBox, SW_HIDE);
    InvalidateRect(f_hWnd, NULL, TRUE);
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
                case IDM_HINTS:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_HINTS), hWnd, Hints);
                    break;
                case IDM_QUIZZTYPE_NARUTO:
                    stop(hWnd);
                    // Read input data
                    init(hmenu, themes::Naruto);
                    break;
                case IDM_QUIZZTYPE_GOT:
                    stop(hWnd);
                    // Read input data
                    init(hmenu, themes::GoT);
                    break;
                case IDM_MODE_TRAINING:
                    s_pCurrentSession->setMode(quizz_mode::TRAINING);
                    toggleMode(hmenu, menuItemTraining);
                    stop(hWnd);
                    break;
                case IDM_MODE_EXAM:
                    s_pCurrentSession->setMode(quizz_mode::TEST);
                    toggleMode(hmenu, menuItemTraining);
                    stop(hWnd);
                    break;
                case IDC_MAIN_BUTTON:
                    s_pCurrentSession->m_gameStarted = EGameState::started;
                    if (s_pCurrentSession->getMode() == quizz_mode::TEST)
                    {
                        s_pCurrentSession->setStart();
                    }
                    UpdateWindow(hWnd);
                    ShowWindow(hEditTextBox, SW_HIDE);
                    ShowWindow(hEditButtonBox, SW_HIDE);
                    ShowWindow(hEditStopButtonBox, SW_SHOW);
                    ShowWindow(hAnswerEditTextBox, SW_SHOW);
                    ShowWindow(hAnswerEditButtonBox, SW_SHOW);
                    ShowWindow(hWndPictureBox, SW_SHOW);
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case IDC_STOP_BUTTON:
                    stop(hWnd);
                    break;
                case IDC_ANSWER_BUTTON:
                    // All processing is done in case WM_PAINT
                    if ((s_pCurrentSession->getMode() == quizz_mode::TEST) && (s_pCurrentSession->get_questionNumber() > 10))
                    {
                        // for the test, limit the question number
                        stop(hWnd);
                    }
                    else
                    {
                        if (s_people_with_gt.size()<1)
                        {
                            init(hmenu, s_pCurrentSession->getTheme());
                        }
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
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
            // Initialize "Hints" menu
            worldMapImage = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_WORLD_MAP));
            if (worldMapImage == NULL)
                MessageBox(hWnd, L"Could not load IDB_WORLD_MAP!", L"Error", MB_OK | MB_ICONEXCLAMATION);
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

            // Create stop Button box
            hEditStopButtonBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Stop game",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                155, 275, 85, 30, hWnd, (HMENU)IDC_STOP_BUTTON, GetModuleHandle(NULL), NULL);
            if (hEditStopButtonBox == NULL)
                MessageBox(hWnd, _T("Could not create edit box."), L"Error", MB_OK | MB_ICONERROR);
            hfStopButtonBox = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hEditStopButtonBox, WM_SETFONT, (WPARAM)hfStopButtonBox, MAKELPARAM(FALSE, 0));

            // Create answer Button box
            hAnswerEditButtonBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Send",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                270, 200, 85, 30, hWnd, (HMENU)IDC_ANSWER_BUTTON, GetModuleHandle(NULL), NULL);
            if (hAnswerEditButtonBox == NULL)
                MessageBox(hWnd, _T("Could not create edit box."), L"Error", MB_OK | MB_ICONERROR);
            hfAnswerButtonBox = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hAnswerEditButtonBox, WM_SETFONT, (WPARAM)hfAnswerButtonBox, MAKELPARAM(FALSE, 0));

            // Create the image associated with the question
            hWndPictureBox = CreateWindowEx(0, L"STATIC", L"Picture Box",
                WS_CHILD | WS_VISIBLE | SS_BITMAP,
                500, 100, 100, 100, hWnd, (HMENU)IDB_CHARACTER, GetModuleHandle(NULL), NULL);

            ShowWindow(hEditTextBox, SW_SHOW);
            ShowWindow(hEditButtonBox, SW_SHOW);
            ShowWindow(hEditStopButtonBox, SW_HIDE);
            ShowWindow(hAnswerEditTextBox, SW_HIDE);
            ShowWindow(hAnswerEditButtonBox, SW_HIDE);
            break;
        // Handling window display
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);

                // Display previous test results
                if (s_pCurrentSession->m_gameStarted == EGameState::finished)
                {
                    string score{ "Score: "};
                    score.append(to_string(s_pCurrentSession->get_scorePercent()));
                    score.append("%");
                    setQuizzAnswerColor(hdc, s_pCurrentSession->get_scorePercent());
                    TextOutA(hdc, 5, 125, score.c_str(), _tcslen(charToWChar(score.c_str())));
                    s_pCurrentSession->resetScore();
                    s_pCurrentSession->resetQuestionNumber();
                    s_pCurrentSession->m_gameStarted = EGameState::not_started;
                }

                // Reset view
                if (s_pCurrentSession->m_gameStarted == EGameState::not_started)
                {
                    RECT rc;
                    TCHAR greeting[] = _T("Hello, welcome in this quizz test!");
                    TCHAR selectTheme[] = _T("Please select a theme, a game type and enter your name");
                    // Here your application is laid out.
                    // in the top left corner.
                    SetTextColor(hdc, RGB(0, 0, 0));
                    TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
                    TextOut(hdc, 5, 25, selectTheme, _tcslen(selectTheme));

                    FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
                    EndPaint(hWnd, &ps);
                }
                else
                {
                    RECT rc2; // initialized through FillRect() call
                    TCHAR startQuizz[] = _T("Starting Quizz!");
                    TextOut(hdc, 5, 100, startQuizz, _tcslen(startQuizz));

                    // Print evaluation
                    // No evaluation before the first question has been asked
                    if ((s_pCurrentSession->get_questionNumber() > 0) && (s_pCurrentSession->getMode() == quizz_mode::TRAINING))
                    {
                        TCHAR answer[30];
                        string evaluation;
                        bool correctness{false};
                        GetDlgItemText(hWnd, IDC_ANSWER_EDIT, answer, 20);
                        evaluation = training_mode_evaluate(s_pCurrentSession.get(), answer, correctness);
                        correctness ? SetTextColor(hdc, RGB(0, 255, 0)) : SetTextColor(hdc, RGB(255, 0, 0));
                        SetBkColor(hdc, 0xc000000);
                        TextOutA(hdc, 25, 140, evaluation.c_str(), _tcslen(charToWChar(evaluation.c_str())));
                    }

                    if (s_pCurrentSession->getMode() == quizz_mode::TRAINING)
                    {
                        // Print question
                        SetBkColor(hdc, RGB(255, 255, 255));
                        SetTextColor(hdc, RGB(0, 0, 0));
                        s_question = training_mode_question(s_pCurrentSession.get(), hWnd);
                        TextOutA(hdc, 25, 120, s_question.c_str(), _tcslen(charToWChar(s_question.c_str())));

                        // Print picture
                        string path = std::get<2>(s_people_with_gt[s_pCurrentSession->m_indexPeople]) == "man" ?
                                                    "pictures\\mysteryman.bmp" : "pictures\\mysterywoman.bmp";
                        std::filesystem::path cwd = std::filesystem::current_path() / path;
                        shadowImage = (HBITMAP)LoadImage(NULL, cwd.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                        if (shadowImage == NULL)
                            MessageBox(hWnd, L"Could not load shadowImage!", L"Error", MB_OK | MB_ICONEXCLAMATION);
                        SendMessage(hWndPictureBox, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)shadowImage);

                        // Print score
                        string Score = to_string(s_pCurrentSession->get_score());
                        Score.append("/");
                        Score.append(to_string(s_pCurrentSession->get_questionNumber() - 1));
                        TextOutA(hdc, 150, 100, Score.c_str(), _tcslen(charToWChar(Score.c_str())));
                    }
                    else // test mode
                    {
                        // Print question
                        SetBkColor(hdc, RGB(255, 255, 255));
                        SetTextColor(hdc, RGB(0, 0, 0));
                        s_question = test_mode_question(s_pCurrentSession.get(), hWnd);
                        TextOutA(hdc, 25, 120, s_question.c_str(), _tcslen(charToWChar(s_question.c_str())));
                    }

                    FillRect(hdc, &rc2, GetSysColorBrush(COLOR_WINDOW+5));
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

// Message handler for about box.
INT_PTR CALLBACK Hints(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcMem, worldMapImage));

        GetObject(worldMapImage, sizeof(bm), &bm);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        EndPaint(hWnd, &ps);
        break;
    }
    return (INT_PTR)FALSE;
}
