
#include "framework.h"
#include "render_context.h"


extern HDC g_hdc;


void initRenderContext(render_context_t& rc)
{
    rc.moveTo = [](int x, int y)
    {
        HDC hdc = g_hdc;
        MoveToEx(hdc, x, y, NULL);
    };

    rc.lineTo = [](int x, int y, int color)
    {
        HDC hdc = g_hdc;
        auto pen = GetStockObject(DC_PEN);
        auto penOrigin = SelectObject(hdc, pen);
        SetDCPenColor(hdc, color);
        LineTo(hdc, x, y);
        SelectObject(hdc, penOrigin);
    };

    rc.rect = [](int x, int y, int cx, int cy, int color)
    {
        HDC hdc = g_hdc;
        auto penOrigin = SelectObject(hdc, GetStockObject(DC_PEN));
        auto brushOrigin = SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCPenColor(hdc, color);
        SetDCBrushColor(hdc, 0xFF000000);

        Rectangle(hdc, x, y, x + cx, y + cy);

        SelectObject(hdc, penOrigin);
        SelectObject(hdc, brushOrigin);
    };

    rc.text = [](RECT *prc, const char* text, int color, int bgColor, int flags)
    {
        HDC hdc = g_hdc;
        SetTextColor(hdc, color);
        SetBkMode(hdc, OPAQUE);
        SetBkColor(hdc, bgColor);

        DrawTextA(hdc, (char*)text, -1, prc, flags);
    };

    rc.calcText = [](RECT* prc, const char* text, int flags)
    {
        HDC hdc = g_hdc;
        DrawTextA(hdc, (char*)text, -1, prc, flags | DT_CALCRECT);
    };
}
