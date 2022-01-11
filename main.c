
#include "main.h"
#include <stdio.h>
#include <commctrl.h>

#include "bv_config.h"
#include "bv_terminal.h"

#include "frame_tab1.h"


HINSTANCE       g_hInst = NULL;
WCHAR           g_szTitle[MAX_LOADSTRING + 1] = { 0 };
WCHAR           g_szWindowClass[MAX_LOADSTRING + 1] = { 0 };
HBRUSH          g_bgBrush = NULL;
HBRUSH          g_bgBrushDebug = NULL;
HDC             g_hdc = NULL;
HANDLE          g_hThread = NULL;
HANDLE          g_hStopEvent = NULL;

/***********************************************************************/

LRESULT CALLBACK    WndProc(
    _In_ HWND hWnd,
    _In_ UINT message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam);
INT_PTR CALLBACK    About(
    _In_ HWND hDlg,
    _In_ UINT message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam);
LRESULT CALLBACK    ButtonProc(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam,
    _In_ UINT_PTR uIdSubclass,
    _In_ DWORD_PTR dwRefData);

ATOM            MyRegisterClass(HINSTANCE hInstance);
BOOL            InitInstance(HINSTANCE hInstance, INT nCmdShow);
VOID            GetTerminalRect(HWND hWnd, RECT* prc);
VOID            UpdateButtonsPos(HWND hWnd);
VOID            GetControlPos(INT index, POINT* ppt);
DWORD WINAPI    ThreadProc(LPVOID lpParameter);

LRESULT         OnCreate(HWND hWnd, CREATESTRUCT* pCS);
VOID            OnDestroy(HWND hWnd);
VOID            OnEraseBackground(HWND hWnd, HDC hdc);
VOID            OnPaint(HWND hWnd, PAINTSTRUCT* pPS, HDC hdc);
LRESULT         OnNotify(NMHDR* pNMHDR);
LRESULT         OnCommand(HWND hWnd, WORD nId, BOOL isMenuItem);
LRESULT         OnCommandNotify(HWND hWnd, HWND hCtl, WORD nCtlId, WORD nCode);

/***********************************************************************/

INT APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TERMINAL, g_szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    g_hStopEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (!g_hStopEvent)
    {
        return FALSE;
    }

    g_hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    if (!g_hThread)
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TERMINAL));

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    CloseHandle(g_hStopEvent);
    CloseHandle(g_hThread);

    return (int)msg.wParam;
}

void SetWindowTitle(HWND hWnd)
{
    char szText[MAX_LOADSTRING] = { 0 };
    LoadStringA(g_hInst, IDC_TERMINAL, szText, MAX_LOADSTRING);

    RECT rc = { 0 };
    GetClientRect(hWnd, &rc);

    //std::stringstream ss;
    //ss 
    //    << szText
    //    << ' '
    //    << '[' << RECT_WIDTH(rc) << 'x' << RECT_HEIGHT(rc) << ']'
    //    << ' '
    //    << '[' << "???" /* g_terminal.context.rc.width()*/ << 'x' << "???" /*g_terminal.context.rc.height()*/ << ']'
    //    ;

    char szTitle[1024] = { 0 };
    sprintf_s(szTitle, 1024 - 1, "%s [%3ix%3i] [%3ix%3i]", szText, RECT_WIDTH(rc), RECT_HEIGHT(rc), TERMINAL_WIDTH, TERMINAL_HEIGHT);
    SetWindowTextA(hWnd, szTitle);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TERMINAL));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)NULL;
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TERMINAL);
    wcex.lpszClassName = g_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, INT nCmdShow)
{
    g_hInst = hInstance;

    auto bmStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

    RECT rc = {
        .left = 0,
        .top = 0,
        .right = TERMINAL_WIDTH + CTL_BUTTON_ZONE_SIZE * 2,
        .bottom = TERMINAL_HEIGHT + CTL_BUTTON_ZONE_SIZE * 2,
    };

    AdjustWindowRect(&rc, bmStyle, TRUE);

    DWORD cx = RECT_WIDTH(rc);
    DWORD cy = RECT_HEIGHT(rc);

    HWND hWnd = CreateWindowExW(0, g_szWindowClass, g_szTitle, bmStyle,
        CW_USEDEFAULT, 0, cx, cy, NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return FALSE;

    g_hdc = GetDC(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void GetTerminalRect(HWND hWnd, RECT* prc)
{
    GetClientRect(hWnd, prc);

    prc->right = prc->left + TERMINAL_WIDTH;
    prc->bottom = prc->top + TERMINAL_HEIGHT;

    OffsetRect(prc, CTL_BUTTON_ZONE_SIZE, CTL_BUTTON_ZONE_SIZE);
}

LRESULT OnCreate(HWND hWnd, CREATESTRUCT* pCS)
{
    // Buttons
    DWORD bmStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
    POINT pt = { 0 };
    for (INT i = 0; i < BUTTON_COUNT; ++i)
    {
        HWND hButton = CreateWindowExA(0, "BUTTON", 0, bmStyle, 0, 0,
            CTL_BUTTON_SIZE, CTL_BUTTON_SIZE, hWnd,
            (HMENU)(UINT_PTR)(CTL_BUTTON_BASE_ID + i), g_hInst, 0);
        if (!hButton)
        {
            return FALSE;
        }

        if (!SetWindowSubclass(hButton, ButtonProc, 0, (DWORD_PTR)NULL))
        {
            return FALSE;
        }
    }

    g_hdc = GetDC(hWnd);

    return TRUE;
}

VOID OnDestroy(HWND hWnd)
{
    if (g_hdc)
    {
        ReleaseDC(hWnd, g_hdc);
        g_hdc = NULL;
    }

    DeleteObject(g_bgBrush);
    g_bgBrush = NULL;

    DeleteObject(g_bgBrushDebug);
    g_bgBrushDebug = NULL;

    SetEvent(g_hStopEvent);
    WaitForSingleObject(g_hThread, INFINITE);

    PostQuitMessage(0);
}

LRESULT OnCommand(HWND hWnd, WORD nId, BOOL isMenuItem)
{
    if (isMenuItem)
    {
        switch (nId)
        {

        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        }
    }

    return 0;
}

LRESULT OnCommandNotify(HWND hWnd, HWND hCtl, WORD nCtlId, WORD nCode)
{
    return 0;
}

VOID OnEraseBackground(HWND hWnd, HDC hdc)
{
    if (g_bgBrush == NULL)
        g_bgBrush = (HBRUSH)CreateSolidBrush(WINDOW_BG_COLOR);
}

VOID OnPaint(HWND hWnd, PAINTSTRUCT* pPS, HDC hdc)
{

}

LRESULT OnNotify(NMHDR* pNMHDR)
{
    uint8_t nButtonIndex = pNMHDR->idFrom - CTL_BUTTON_BASE_ID;

    if (nButtonIndex >= 0 && nButtonIndex < BUTTON_COUNT)
    {
        switch (pNMHDR->code)
        {
        case BN_DOWN:
            BVK_SetState(nButtonIndex, BS_DOWN);
            break;

        case BN_UP:
            BVK_SetState(nButtonIndex, BS_UP);
            break;
        }
    }

    return (LRESULT)NULL;
}

LRESULT CALLBACK WndProc(
    _In_ HWND hWnd,
    _In_ UINT message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{
    switch (message)
    {

    case WM_CREATE:
        return OnCreate(hWnd, (CREATESTRUCT*)lParam);

    case WM_DESTROY:
        OnDestroy(hWnd);
        break;

    case WM_SHOWWINDOW:
        BVT_Init();
        break;

    case WM_SIZE:
    case WM_SIZING:
    {
        RECT rc = { 0 };
        GetTerminalRect(hWnd, &rc);

        SetWindowTitle(hWnd);

        UpdateButtonsPos(hWnd);

        return TRUE;
    }

    case WM_ERASEBKGND:
        OnEraseBackground(hWnd, (HDC)wParam);
        return TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        OnPaint(hWnd, &ps, hdc);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_NOTIFY:
        return OnNotify((NMHDR*)lParam);

    case WM_COMMAND:
        return lParam
            ? OnCommandNotify(hWnd, (HWND)lParam, LOWORD(wParam), HIWORD(wParam))
            : OnCommand(hWnd, LOWORD(wParam), HIWORD(wParam) == 0);

    case WM_RBUTTONUP:
    {
        if (g_bgBrushDebug == NULL)
            g_bgBrushDebug = CreateSolidBrush(RGB(255, 0, 255));

        RECT rc = { 0 };
        GetClientRect(hWnd, &rc);

        InflateRect(&rc, -CTL_BUTTON_ZONE_SIZE, -CTL_BUTTON_ZONE_SIZE);
        FillRect(g_hdc, &rc, g_bgBrushDebug);
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return (LRESULT)NULL;
}

INT_PTR CALLBACK About(
    _In_ HWND hDlg,
    _In_ UINT message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
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

void GetControlPos(INT index, POINT* ppt)
{
    point_t pt = { 0 };
    BVT_CalcButtonPos(&pt, index, -CTL_BUTTON_ZONE_SIZE / 2);

    ppt->x = (LONG)(pt.x + CTL_BUTTON_ZONE_SIZE - CTL_BUTTON_SIZE / 2);
    ppt->y = (LONG)(pt.y + CTL_BUTTON_ZONE_SIZE - CTL_BUTTON_SIZE / 2);
}

void UpdateButtonsPos(HWND hWnd)
{
    POINT pt = { 0 };

    for (uint8_t i = 0; i < BUTTON_COUNT; ++i)
    {
        GetControlPos(i, &pt);

        HWND hBtn = GetDlgItem(hWnd, CTL_BUTTON_BASE_ID + i);
        SetWindowPos(hBtn, 0, pt.x, pt.y, 0, 0, SWP_NOSIZE);
    }
}

DWORD WINAPI ThreadProc(
    LPVOID lpParameter)
{
    BVT_PushFrame(FrameTab1Proc);

    while (WaitForSingleObject(g_hStopEvent, 1) == WAIT_TIMEOUT)
    {
        BVT_Magic();
    }

    return 0;
}

LRESULT CALLBACK ButtonProc(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam,
    _In_ UINT_PTR uIdSubclass,
    _In_ DWORD_PTR dwRefData)
{
    HWND hParentWnd = GetParent(hWnd);
    DWORD nId = GetDlgCtrlID(hWnd);

    NMHDR nmhdr = { 0 };
    nmhdr.hwndFrom = hWnd;
    nmhdr.idFrom = nId;

    switch (uMsg)
    {

    case WM_LBUTTONDOWN:
        nmhdr.code = BN_DOWN;
        SendMessage(hParentWnd, WM_NOTIFY, nId, (LPARAM)&nmhdr);
        break;

    case WM_LBUTTONUP:
        nmhdr.code = BN_UP;
        SendMessage(hParentWnd, WM_NOTIFY, nId, (LPARAM)&nmhdr);
        break;

    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}