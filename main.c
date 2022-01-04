
#include "main.h"
#include <stdio.h>

#include "bv_config.h"
#include "bv_terminal.h"


HINSTANCE       g_hInst;
WCHAR           g_szTitle[MAX_LOADSTRING];
WCHAR           g_szWindowClass[MAX_LOADSTRING];
HBRUSH          g_bgBrush = NULL;
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
void        GetTerminalRect(HWND hWnd, RECT* prc);
void        UpdateButtonsPos(HWND hWnd);
void        GetControlPos(int index, POINT* ppt);

LRESULT     OnCreate(HWND hWnd, CREATESTRUCT* pCS);
LRESULT     OnCommand(HWND hWnd, WORD nId, BOOL isMenuItem);
LRESULT     OnCommandNotify(HWND hWnd, HWND hCtl, WORD nCtlId, WORD nNotifyCode);
LRESULT     OnEraseBackground(HWND hWnd, HDC hdc);
LRESULT     OnPaint(HWND hWnd, PAINTSTRUCT* pPS, HDC hdc);

/***********************************************************************/

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
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

    auto dwStyle = WS_OVERLAPPEDWINDOW;
    RECT rc = { 
        .left = 0,
        .top = 0,
        .right = TERMINAL_WIDTH + CTL_BUTTON_ZONE_SIZE * 2,
        .bottom = TERMINAL_WIDTH + CTL_BUTTON_ZONE_SIZE * 2,
    };

    AdjustWindowRect(&rc, dwStyle, TRUE);

    DWORD cx = RECT_WIDTH(rc);
    DWORD cy = RECT_HEIGHT(rc);

    HWND hWnd = CreateWindowExW(0, g_szWindowClass, g_szTitle, dwStyle,
        CW_USEDEFAULT, 0, cx, cy, NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return FALSE;

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
    RECT rc = { 0 };
    GetTerminalRect(hWnd, &rc);

    DWORD dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
    POINT pt = { 0 };

    for (INT i = 0; i < BUTTON_COUNT; ++i)
    {
        GetControlPos(i, &pt);

        CreateWindowExA(0, "BUTTON", 0, dwStyle, pt.x, pt.y,
            CTL_BUTTON_SIZE, CTL_BUTTON_SIZE, hWnd,
            (HMENU)(UINT_PTR)(CTL_BUTTON_BASE_ID + i), g_hInst, 0);
    }

    BVT_Init();

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
    WORD index = nCtlId - CTL_BUTTON_BASE_ID;

    if (index >= 0 && index < BUTTON_COUNT)
    {
#define TEXT_LEN    7
#define BTN_LEN     2

        char text[TEXT_LEN + 1] = { "OSB " };
        char buff[BTN_LEN + 1] = { 0 };
        _itoa_s(index + 1, buff, BTN_LEN + 1, 10);
        if (index + 1 < 10)
            strcat_s(text, TEXT_LEN + 1, "0");
        strcat_s(text, TEXT_LEN + 1, buff);
        SetWindowTextA(hWnd, text);

        //BVG_InvalidateRect(0, 1);
        //InvalidateRect(hWnd, NULL, TRUE);

        frame_proc_f proc = BVT_GetTopFrame();
        if (proc)
        {
            proc(FM_NOTIFICATION, NP_CLICK(index));
        }
    }
   
    return 0;
}

LRESULT OnEraseBackground(HWND hWnd, HDC hdc)
{
    if (g_bgBrush == NULL)
        g_bgBrush = (HBRUSH)CreateSolidBrush(WINDOW_BG_COLOR);

    RECT rc = { 0 };

    // Wnd
    GetClientRect(hWnd, &rc);
    FillRect(hdc, &rc, g_bgBrush);

    // Terminal
    GetTerminalRect(hWnd, &rc);
    SetDCBrushColor(hdc, RGB(0, 0, 0));
    FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));

    return TRUE;
}

LRESULT OnPaint(HWND hWnd, PAINTSTRUCT * pPS, HDC hdc)
{
    RECT rc = { 0 };
    GetTerminalRect(hWnd, &rc);

    g_hdc = hdc;
    BVT_InvalidateRect(0, 1);
    g_hdc = NULL;

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

    case WM_ERASEBKGND:
        return OnEraseBackground(hWnd, (HDC)wParam);

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

    case WM_SIZING:
    case WM_SIZE:
    {
        RECT rc = { 0 };
        GetTerminalRect(hWnd, &rc);

        SetWindowTitle(hWnd);

        // Terminal - Background
        //HDC hdc = GetDC(hWnd);
        //GetTerminalRect(hWnd, &rc);
        //SetDCBrushColor(hdc, RGB(0, 0, 0));
        //FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
        //ReleaseDC(hWnd, hdc);

        // Terminal - Overlay
        //g_terminal.size(_torect(rc));
        //

        
        //UpdateButtonsPos(hWnd);

        return TRUE;
    }

    case WM_DESTROY:
        DeleteObject(g_bgBrush);
        g_bgBrush = NULL;

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

void GetControlPos(int index, POINT* ppt)
{
    point_t pt = { 0 };
    BVT_CalcButtonPos(&pt, index, -CTL_BUTTON_ZONE_SIZE / 2);

    ppt->x = (LONG)(pt.x - CTL_BUTTON_SIZE / 2);
    ppt->y = (LONG)(pt.y - CTL_BUTTON_SIZE / 2);
}

void UpdateButtonsPos(HWND hWnd)
{
    POINT pt = { 0 };

    for (auto i = 0; i < BUTTON_COUNT; ++i)
    {
        GetControlPos(i, &pt);

        HWND hBtn = GetDlgItem(hWnd, CTL_BUTTON_BASE_ID + i);
        SetWindowPos(hBtn, 0, pt.x, pt.y, 0, 0, SWP_NOSIZE);
    }
}
