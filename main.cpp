
#include "main.h"
#include "Terminal.h"


// Глобальные переменные:
HINSTANCE   g_hInst;                                    // текущий экземпляр
WCHAR       g_szTitle[MAX_LOADSTRING];                  // текст строки заголовка
WCHAR       g_szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HDC         g_hdc = NULL;
Terminal    g_terminal;

/***********************************************************************/


ATOM                MyRegisterClass(
    _In_ HINSTANCE hInstance);
BOOL                InitInstance(
    _In_ HINSTANCE hInstance, 
    _In_ INT nCmdShow);
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
void                CalcButtonPos(const RECT* prc, int buttonIndex, POINT* ppt);
void                UpdateButtonsPos(HWND hWnd);


/***********************************************************************/


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

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


ATOM MyRegisterClass(
    _In_ HINSTANCE hInstance)
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
    wcex.hbrBackground = CreateSolidBrush(RGB(20, 20, 20));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TERMINAL);
    wcex.lpszClassName = g_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(
    _In_ HINSTANCE hInstance, 
    _In_ INT nCmdShow)
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

    HWND hWnd = CreateWindowW(g_szWindowClass, g_szTitle, dwStyle,
        CW_USEDEFAULT, 0, cx, cy, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
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
    {
        RECT rc = { 0 };
        GetClientRect(hWnd, &rc);

        render_context_t renderContext;
        initRenderContext(renderContext);
        g_terminal.Init(renderContext);

        /*POINT pt = { 0 };

        auto dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
        auto buttonsCount = (buttonsX + buttonsY) * 2;

        for (auto i = 0; i < buttonsCount; ++i)
        {
            char title[3] = { 0 };
            _itoa_s(i + 1, title, 10);

            CalcButtonPos(&rc, i, &pt);

            CreateWindowExA(NULL, "BUTTON", title, dwStyle, pt.x, pt.y, buttonSize, buttonSize, hWnd,
                (HMENU)((BASE_BUTTON_ID + i)), g_hInst, NULL);
        }*/
        return TRUE;
    }
    break;

    case WM_COMMAND:
    {
        if (lParam == NULL)
        {
            // Menu & Accelerator
            if (HIWORD(wParam))
            {
                // Accelerator
                auto ida = LOWORD(wParam);
            }
            else
            {
                // Menu
                auto idm = LOWORD(wParam);
                switch (idm)
                {
                case IDM_ABOUT:
                    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
        }
        else
        {
            // Element
            auto id = LOWORD(wParam);
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
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        auto hdc = BeginPaint(hWnd, &ps);

        RECT rc = { 0 };
        GetClientRect(hWnd, &rc);

        auto cx = RECT_WIDTH(rc) - CTL_BUTTON_ZONE_SIZE * 2;
        auto cy = RECT_HEIGHT(rc) - CTL_BUTTON_ZONE_SIZE * 2;

        g_terminal.m_context.width = cx;
        g_terminal.m_context.height = cy;

        auto hdcMem = CreateCompatibleDC(hdc);
        g_hdc = hdcMem;
        auto hBmp = CreateCompatibleBitmap(hdc, cx, cy);
        auto hBmpOrigin = SelectObject(hdcMem, hBmp);
        {
            g_terminal.Render();

            BitBlt(hdc, CTL_BUTTON_ZONE_SIZE, CTL_BUTTON_ZONE_SIZE, cx, cy, 
                hdcMem, 0, 0, SRCCOPY);
        }
        SelectObject(hdcMem, hBmpOrigin);
        DeleteObject(hBmp);
        DeleteDC(hdcMem);
        g_hdc = NULL;

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_SIZING:
    case WM_SIZE:
        //UpdateButtonsPos(hWnd);
        return TRUE;

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

//
//void UpdateButtonsPos(HWND hWnd)
//{
//    POINT pt = { 0 };
//    RECT rc = { 0 };
//    GetClientRect(hWnd, &rc);
//
//    for (auto i = 0; i < BUTTON_COUNT; ++i)
//    {
//        CalcButtonPos(&rc, i, &pt);
//
//        auto hBtn = GetDlgItem(hWnd, BASE_BUTTON_ID + i);
//        SetWindowPos(hBtn, NULL, pt.x, pt.y, NULL, NULL, SWP_NOSIZE);
//    }
//}
//
//
//void CalcButtonPos(const RECT* prc, int buttonIndex, POINT* ppt)
//{
//    auto cx = prc->right - prc->left - buttonOffset * 2;
//    auto cy = prc->bottom - prc->top - buttonOffset * 2;
//
//    auto stepX = (cx - offset * 2) / BUTTON_COUNT_X;
//    auto stepY = (cy - offset * 2) / BUTTON_COUNT_Y;
//
//    if (buttonIndex == 0)
//    {
//        // top
//        ppt->x = buttonOffset + offset - stepX / 2 - buttonSize / 2;
//        ppt->y = (buttonOffset - buttonSize) / 2;
//    }
//    else if (buttonIndex == BUTTON_COUNT_X)
//    {
//        // right
//        ppt->x = buttonOffset + cx + (buttonOffset - buttonSize) / 2;
//        ppt->y = buttonOffset + offset - stepY / 2 - buttonSize / 2;
//    }
//    else if (buttonIndex == BUTTON_COUNT_X + BUTTON_COUNT_Y)
//    {
//        // bottom
//        ppt->x = buttonOffset + cx + buttonSize / 2;
//        ppt->y = buttonOffset + cy + (buttonOffset - buttonSize) / 2;
//    }
//    else if (buttonIndex == BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)
//    {
//        // left
//        ppt->x = (buttonOffset - buttonSize) / 2;
//        ppt->y = buttonOffset + offset + cy;
//    }
//
//    // Step by Side
//    if (buttonIndex < BUTTON_COUNT_X)                                   // top
//        ppt->x += stepX;
//    else if (buttonIndex < BUTTON_COUNT_X + BUTTON_COUNT_Y)             // right
//        ppt->y += stepY;
//    else if (buttonIndex < BUTTON_COUNT_X * 2 + BUTTON_COUNT_Y)         // bottom
//        ppt->x -= stepX;
//    else if (buttonIndex < (BUTTON_COUNT_X + BUTTON_COUNT_Y) * 2)       // left
//        ppt->y -= stepY;
//}
