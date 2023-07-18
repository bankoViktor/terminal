/*
 * File     main.c
 * Date     15.07.2023
 */

#include "main.h"
#include <stdio.h>

#include "core/inc/bv_config.h"
#include "core/inc/bv_terminal.h"
#include "frames/inc/frame_tab1.h"
#include "user_data.h"


HINSTANCE       g_hInst = NULL;
WCHAR           g_szTitle[MAX_LOADSTRING];
WCHAR           g_szWindowClass[MAX_LOADSTRING];
HBRUSH          g_bgBrushDebug = NULL;
HDC             g_hdc = NULL;


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

ATOM        MyRegisterClass(HINSTANCE hInstance);
BOOL        InitInstance(HINSTANCE hInstance, INT nCmdShow);
VOID        GetTerminalRect(HWND hWnd, RECT* prc);
VOID        GetControlPos(INT nIndex, POINT* ppt);
VOID        GetButtonPos(POINT* ppt, UINT nIndex, INT nOffset);

LRESULT     OnCreate(HWND hWnd, CREATESTRUCT* pCS);
LRESULT     OnCommand(HWND hWnd, WORD nId, BOOL isMenuItem);
LRESULT     OnCommandNotify(HWND hWnd, HWND hCtl, WORD nCtlId, WORD nNotifyCode);
LRESULT     OnPaint(HWND hWnd, PAINTSTRUCT* pPS, HDC hdc);

/***********************************************************************/

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance,
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

    return (INT)msg.wParam;
}

VOID SetWindowTitle(HWND hWnd)
{
    CHAR szText[MAX_LOADSTRING] = { 0 };
    LoadStringA(g_hInst, IDC_TERMINAL, szText, MAX_LOADSTRING);

    RECT rc = { 0 };
    GetClientRect(hWnd, &rc);

    CHAR szTitle[TITLE_LENGTH_MAX] = { 0 };
    sprintf_s(szTitle, TITLE_LENGTH_MAX, "%s [%3ix%3i] [%3ix%3i]",
        szText, RECT_WIDTH(rc), RECT_HEIGHT(rc), TERMINAL_WIDTH, TERMINAL_HEIGHT);

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
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(WINDOW_BG_COLOR);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TERMINAL);
    wcex.lpszClassName = g_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, INT nCmdShow)
{
    g_hInst = hInstance;

    DWORD bmStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

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

void TerminalInit()
{
    memset(&g_UserData, 0, sizeof(g_UserData));

    // Init User Data
    g_UserData.bBool = _TRUE;
    g_UserData.bSelectable = 2;
    g_UserData.bMode = _FALSE;
    g_UserData.wInputNumber = 1234;
    g_UserData.dInputDouble = 0.75;

    strcpy_s(g_UserData.szTestTextValue, 7, "mark-1");

    // Init Terminal Core
    BVT_Init(FrameTab1Proc);
}


LRESULT OnCreate(HWND hWnd, CREATESTRUCT* pCS)
{
    RECT rc = { 0 };
    GetTerminalRect(hWnd, &rc);

    DWORD bmStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD;
    POINT pt = { 0 };

    for (INT i = 0; i < BUTTONS_COUNT; ++i)
    {
        GetControlPos(i, &pt);

        CreateWindowExA(0, "BUTTON", 0, bmStyle, pt.x, pt.y,
            CTL_BUTTON_SIZE, CTL_BUTTON_SIZE, hWnd,
            (HMENU)(UINT_PTR)(CTL_BUTTON_BASE_ID + i), g_hInst, 0);
    }

    TerminalInit();

    return TRUE;
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

LRESULT OnCommandNotify(HWND hWnd, HWND hCtl, WORD nCtlId, WORD nNotifyCode)
{
    WORD nIndex = nCtlId - CTL_BUTTON_BASE_ID;

    if (nIndex >= 0 && nIndex < BUTTONS_COUNT)
    {
        // Send Button Up notification to current frame
        frame_proc_f proc = BVT_GetTopFrame();
        _SendMsgButtonUp(proc, nIndex);
    }

    return 0;
}

LRESULT OnPaint(HWND hWnd, PAINTSTRUCT* pPS, HDC hdc)
{
    // Draw display rect
    RECT rc = { 0 };
    GetTerminalRect(hWnd, &rc);
    FillRect(hdc, &rc, GetStockObject(BLACK_BRUSH));

    // Draw terminal
    BVT_InvalidateRect(NULL, TRUE);

    return 0;
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

    case WM_COMMAND:
        return lParam
            ? OnCommandNotify(hWnd, (HWND)lParam, LOWORD(wParam), HIWORD(wParam))
            : OnCommand(hWnd, LOWORD(wParam), HIWORD(wParam) == 0);

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        LRESULT result = OnPaint(hWnd, &ps, hdc);
        EndPaint(hWnd, &ps);
        return result;
    }

    case WM_RBUTTONUP:
    {
        if (g_bgBrushDebug == NULL)
            g_bgBrushDebug = CreateSolidBrush(RGB(255, 0, 255));

        RECT rc = { 0 };
        GetClientRect(hWnd, &rc);

        InflateRect(&rc, -CTL_BUTTON_ZONE_SIZE, -CTL_BUTTON_ZONE_SIZE);
        FillRect(g_hdc, &rc, g_bgBrushDebug);
        return 0;
    }

    case WM_DESTROY:
        if (g_hdc)
        {
            ReleaseDC(hWnd, g_hdc);
            g_hdc = NULL;
        }

        DeleteObject(g_bgBrushDebug);
        g_bgBrushDebug = NULL;

        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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

VOID GetControlPos(INT nIndex, POINT* ppt)
{
    POINT pt = { 0 };
    GetButtonPos(&pt, nIndex, -CTL_BUTTON_ZONE_SIZE / 2);

    ppt->x = CTL_BUTTON_ZONE_SIZE + pt.x - CTL_BUTTON_SIZE / 2;
    ppt->y = CTL_BUTTON_ZONE_SIZE + pt.y - CTL_BUTTON_SIZE / 2;
}

VOID GetButtonPos(POINT* pPt, UINT nButtonIndex, INT nOffset)
{
    RECT    rc = { 0, 0, TERMINAL_WIDTH, TERMINAL_HEIGHT };
    INT     nWidth = RECT_WIDTH(rc) - SAFE_OFFSET_LEFT - SAFE_OFFSET_RIGHT + BUTTON_STRECH_X * 2;
    INT     nHeight = RECT_HEIGHT(rc) - SAFE_OFFSET_TOP - SAFE_OFFSET_BOTTOM + BUTTON_STRECH_Y * 2;
    FLOAT   fStepX = (FLOAT)nWidth / (BUTTON_COUNT_X + 1);
    FLOAT   fStepY = (FLOAT)nHeight / (BUTTON_COUNT_Y + 1);

    if (nButtonIndex < BUTTONS_RIGHT_OFFSET)
    {
        // Top
        pPt->x = rc.left + SAFE_OFFSET_LEFT - BUTTON_STRECH_X + BUTTON_MOVE_X + (INT)(fStepX * (nButtonIndex + 1));
        pPt->y = rc.top + nOffset;
    }
    else if (nButtonIndex < BUTTONS_BOTTOM_OFFSET)
    {
        // Right
        pPt->x = rc.right - 1 - nOffset;
        pPt->y = rc.top + SAFE_OFFSET_TOP - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (INT)(fStepY * (nButtonIndex - BUTTONS_RIGHT_OFFSET + 1));
    }
    else if (nButtonIndex < BUTTONS_LEFT_OFFSET)
    {
        // Bottom
        pPt->x = rc.left + SAFE_OFFSET_LEFT - BUTTON_STRECH_X + BUTTON_MOVE_X + (INT)(fStepX * (BUTTON_COUNT_X - nButtonIndex + BUTTONS_BOTTOM_OFFSET));
        pPt->y = rc.bottom - 1 - nOffset;
    }
    else if (nButtonIndex < BUTTONS_COUNT)
    {
        // Left
        pPt->x = rc.left + nOffset;
        pPt->y = rc.top + SAFE_OFFSET_TOP - BUTTON_STRECH_Y + BUTTON_MOVE_Y + (INT)(fStepY * (BUTTON_COUNT_Y - nButtonIndex + BUTTONS_LEFT_OFFSET));
    }
}

VOID GetTerminalRect(HWND hWnd, RECT* prc)
{
    GetClientRect(hWnd, prc);

    prc->right = prc->left + TERMINAL_WIDTH;
    prc->bottom = prc->top + TERMINAL_HEIGHT;

    OffsetRect(prc, CTL_BUTTON_ZONE_SIZE, CTL_BUTTON_ZONE_SIZE);
}
