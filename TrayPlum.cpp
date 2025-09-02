// TrayPlum.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "TrayPlum.h"

#define MAX_LOADSTRING 100

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
NOTIFYICONDATA nid;
HMENU popupMenu;

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Settings(HWND, UINT, WPARAM, LPARAM);
LPCWSTR sTip = _T("Tu można coś napisać, ale po co?");

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRAYPLUM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRAYPLUM));

    MSG msg;

    // Główna pętla komunikatów:
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
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAYICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRAYPLUM);
    wcex.lpszClassName  = szWindowClass;
    // TODO: dorobić małą ikonę: wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej


   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   popupMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_POPUPMENU));

   nid.cbSize = sizeof(NOTIFYICONDATA);
   nid.hWnd = hWnd;
   nid.uID = ID_TRAY;
   nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   nid.uCallbackMessage = MSG_TRAY;
   nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TRAYICON));
   lstrcpy(nid.szTip, sTip);

   SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, 300, 300, SWP_NOMOVE);
   RegisterHotKey(hWnd, ID_HOTKEY, MOD_SHIFT, VK_F1);

   LONG style = GetWindowLong(hWnd, GWL_EXSTYLE);
   SetWindowLong(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
   LONG alpha = 166;
   SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);

   Shell_NotifyIcon(NIM_ADD, &nid);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_SETTINGS:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd, Settings);
                break;
            case IDM_SHOW:
                if (IsWindowVisible(hWnd))
                {
                    ShowWindow(hWnd, SW_HIDE);
                }
                else
                {
                    ShowWindow(hWnd, SW_SHOW);
                }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_HOTKEY:
        if (IsWindowVisible(hWnd))
        {
            ShowWindow(hWnd, SW_HIDE);
        }
        else
        {
            ShowWindow(hWnd, SW_SHOW);
        }
        break;
    case WM_LBUTTONDOWN:
    {
        LONG style = GetWindowLong(hWnd, GWL_EXSTYLE);
        SetWindowLong(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
        LONG alpha = 127;
        SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
    }
    break;
    case WM_RBUTTONUP:
    {
        LONG style = GetWindowLong(hWnd, GWL_EXSTYLE);
        SetWindowLong(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
        LONG alpha = 255;
        SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CLOSE:
        ShowWindow(hWnd, SW_HIDE);
        break;
    case WM_DESTROY:
        NOTIFYICONDATA nidx;
        nidx.cbSize = sizeof(NOTIFYICONDATA);
        nidx.hWnd = hWnd;
        nidx.uID = ID_TRAY;
        nidx.uFlags = 0;
        Shell_NotifyIcon(NIM_DELETE, &nidx);

        PostQuitMessage(0);
        break;
    case MSG_TRAY:
        if (wParam == ID_TRAY)
        {
            switch (lParam)
            {
            case WM_LBUTTONDOWN:
                break;
            case WM_RBUTTONDOWN:
                POINT p;
                GetCursorPos(&p);
                ClientToScreen(hWnd, &p);
                TrackPopupMenu(GetSubMenu(popupMenu, 0), 0, p.x, p.y, 0, hWnd, NULL);
                break;
            }
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Procedura obsługi komunikatów dla okna informacji o programie.
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

INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
