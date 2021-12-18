
#include "main.h"
#include "terminal_config.h"
#include "terminal.h"

#include <sstream>


HINSTANCE   g_hInst;
WCHAR       g_szTitle[MAX_LOADSTRING];
WCHAR       g_szWindowClass[MAX_LOADSTRING];
HDC         g_hdc = NULL;
terminal_t  g_terminal;

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

    auto hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TERMINAL));

    MSG msg;

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

void SetWindowTitle(HWND hWnd)
{
    char szText[MAX_LOADSTRING] = { 0 };
    LoadStringA(g_hInst, IDC_TERMINAL, szText, MAX_LOADSTRING);

    RECT rc = { 0 };
    GetClientRect(hWnd, &rc);

    std::stringstream ss;
    ss 
        << szText
        << ' '
        << '[' << RECT_WIDTH(rc) << 'x' << RECT_HEIGHT(rc) << ']'
        << ' '
        << '[' << g_terminal.m_context.rc.width() << 'x' << g_terminal.m_context.rc.height() << ']';

    SetWindowTextA(hWnd, ss.str().c_str());
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
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

    AdjustWindowRect(&rc, dwStyle, true);

    auto cx = RECT_WIDTH(rc);
    auto cy = RECT_HEIGHT(rc);

    auto hWnd = CreateWindowW(g_szWindowClass, g_szTitle, dwStyle,
        CW_USEDEFAULT, 0, cx, cy, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void GetTerminalRect(HWND hWnd, RECT* prc)
{
    GetClientRect(hWnd, prc);
    InflateRect(prc, -CTL_BUTTON_ZONE_SIZE, -CTL_BUTTON_ZONE_SIZE);
}

LRESULT OnCreate(HWND hWnd, CREATESTRUCT* pCS)
{
    RECT rc = { 0 };
    GetTerminalRect(hWnd, &rc);

    render_context_t renderContext;
    initRenderContext(renderContext, _torect(rc));

    g_terminal.init(renderContext);

    auto dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;

    POINT pt = { 0 };

    for (auto i = 0; i < BUTTON_COUNT; ++i)
    {
        GetControlPos(i, &pt);

        CreateWindowExA(NULL, "BUTTON", NULL, dwStyle, pt.x, pt.y,
            CTL_BUTTON_SIZE, CTL_BUTTON_SIZE, hWnd,
            (HMENU)(UINT_PTR)(CTL_BUTTON_BASE_ID + i), g_hInst, NULL);
    }

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
    /*auto buttonsCount = (buttonsX + buttonsY) * 2;
            if (id >= BASE_BUTTON_ID && BASE_BUTTON_ID + buttonsCount)
            {
                auto index = id - BASE_BUTTON_ID;
                auto buttonNumb = index + 1;

                char text[256] = { "OSB " };
                char numb[3] = { 0 };
                _itoa_s(buttonNumb, numb, 10);
                if (buttonNumb < 10)
                    strcat_s(text, "0");
                strcat_s(text, numb);
                SetWindowTextA(hWnd, text);

                g_terminal.Active(index);
            }*/
    return 0;
}

LRESULT OnEraseBackground(HWND hWnd, HDC hdc)
{
    RECT rc = { 0 };

    // Wnd
    GetClientRect(hWnd, &rc);
    FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));

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
    g_terminal.render();
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
            : OnCommand(hWnd, LOWORD(wParam), HIWORD(wParam) != 0);

    case WM_ERASEBKGND:
        return OnEraseBackground(hWnd, (HDC)wParam);

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(hWnd, &ps);
        auto result = OnPaint(hWnd, &ps, hdc);
        EndPaint(hWnd, &ps);
        return result;
    }

    case WM_SIZING:
    case WM_SIZE:
    {
        RECT rc = { 0 };
        GetTerminalRect(hWnd, &rc);
        g_terminal.m_context.rc = _torect(rc);
        UpdateButtonsPos(hWnd);

        SetWindowTitle(hWnd);
        return TRUE;
    }

    case WM_DESTROY:
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
    point_t pt;
    g_terminal.calcButtonPos(index, pt, -CTL_BUTTON_ZONE_SIZE / 2);
    ppt->x = (LONG)pt.x - CTL_BUTTON_SIZE / 2;
    ppt->y = (LONG)pt.y - CTL_BUTTON_SIZE / 2;
}

void UpdateButtonsPos(HWND hWnd)
{
    POINT pt = { 0 };

    for (auto i = 0; i < BUTTON_COUNT; ++i)
    {
        GetControlPos(i, &pt);

        auto hBtn = GetDlgItem(hWnd, CTL_BUTTON_BASE_ID + i);
        SetWindowPos(hBtn, NULL, pt.x, pt.y, NULL, NULL, SWP_NOSIZE);
    }
}
