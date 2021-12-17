
#include "framework.h"
#include "render_context.h"


extern HDC g_hdc;

int convertToTextFlags(HorizontalAligment horizAlign, VerticalAligment vertAlign)
{
    auto result = DT_NOPREFIX | DT_HIDEPREFIX;

    // Horizontal 
    result &= ~(DT_LEFT | DT_CENTER | DT_RIGHT);
    switch (horizAlign)
    {
    case Left:
        result |= DT_LEFT;
        break;
    case Center:
        result |= DT_CENTER;
        break;
    case Right:
        result |= DT_RIGHT;
        break;
    }

    // Vertical 
    result &= ~(DT_TOP | DT_VCENTER | DT_BOTTOM);
    switch (vertAlign)
    {
    case Top:
        result |= DT_TOP;
        break;
    case Middle:
        result |= DT_VCENTER;
        break;
    case Bottom:
        result |= DT_BOTTOM;
        break;
    }

    return result;
}


void initRenderContext(render_context_t& rc)
{
    rc.moveTo = [](int x, int y)
    {
        auto hdc = g_hdc;
        MoveToEx(hdc, x, y, NULL);
    };

    rc.lineTo = [](int x, int y, int color)
    {
        auto hdc = g_hdc;
        auto pen = GetStockObject(DC_PEN);
        auto penOrigin = SelectObject(hdc, pen);
        SetDCPenColor(hdc, color);
        LineTo(hdc, x, y);
        SelectObject(hdc, penOrigin);
    };

    rc.rect = [](int x, int y, int cx, int cy, int color)
    {
        auto hdc = g_hdc;
        auto penOrigin = SelectObject(hdc, GetStockObject(DC_PEN));
        auto brushOrigin = SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCPenColor(hdc, color);
        SetDCBrushColor(hdc, 0xFF000000);

        Rectangle(hdc, x, y, x + cx, y + cy);

        SelectObject(hdc, penOrigin);
        SelectObject(hdc, brushOrigin);
    };

    rc.text = [](RECT *prc, const char* text, int color, int bgColor, HorizontalAligment horizAlign, VerticalAligment vertAlign)
    {
        auto hdc = g_hdc;
        auto flags = convertToTextFlags(horizAlign, vertAlign);

        SetTextColor(hdc, color);
        SetBkMode(hdc, OPAQUE);
        SetBkColor(hdc, bgColor);
        DrawTextA(hdc, (char*)text, -1, prc, flags);
    };

    rc.calcText = [](RECT* prc, const char* text, HorizontalAligment horizAlign, VerticalAligment vertAlign)
    {
        auto hdc = g_hdc;
        auto flags = convertToTextFlags(horizAlign, vertAlign);

        DrawTextA(hdc, (char*)text, -1, prc, flags | DT_CALCRECT);
    };
}
