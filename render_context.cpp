
#include "framework.h"
#include "render_context.h"

extern HDC g_hdc;

UINT convertToTextFlags(horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign)
{
    UINT result = DT_NOPREFIX | DT_HIDEPREFIX;

    // Horizontal 
    result &= ~(DT_LEFT | DT_CENTER | DT_RIGHT);
    switch (horizAlign)
    {
    case horizontal_aligment_t::Left:
        result |= DT_LEFT;
        break;
    case horizontal_aligment_t::Center:
        result |= DT_CENTER;
        break;
    case horizontal_aligment_t::Right:
        result |= DT_RIGHT;
        break;
    }

    // Vertical 
    result &= ~(DT_TOP | DT_VCENTER | DT_BOTTOM);
    switch (vertAlign)
    {
    case vertical_aligment_t::Top:
        result |= DT_TOP;
        break;
    case vertical_aligment_t::Middle:
        result |= DT_VCENTER;
        break;
    case vertical_aligment_t::Bottom:
        result |= DT_BOTTOM;
        break;
    }

    return result;
}

void initRenderContext(render_context_t& context, const rect_t& rc)
{
    context.rc = rc;

    context.moveTo = [](int16_t x, int16_t y)
    {
        auto hdc = g_hdc;
        MoveToEx(hdc, (int)x, (int)y, NULL);
    };

    context.lineTo = [](int16_t x, int16_t y, color_t color)
    {
        auto hdc = g_hdc;
        auto pen = GetStockObject(DC_PEN);
        auto penOrigin = SelectObject(hdc, pen);
        SetDCPenColor(hdc, (COLORREF)color);
        LineTo(hdc, (int)x, (int)y);
        SelectObject(hdc, penOrigin);
    };

    context.rect = [](const rect_t& rc, color_t color)
    {
        auto hdc = g_hdc;
        auto hPenOrigin = SelectObject(hdc, GetStockObject(DC_PEN));
        auto hBrushOrigin = SelectObject(hdc, GetStockObject(NULL_BRUSH));

        SetDCPenColor(hdc, color);
        Rectangle(hdc, (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);

        SelectObject(hdc, hPenOrigin);
        SelectObject(hdc, hBrushOrigin);
    };

    context.calcText = [](rect_t& rc, const char* text, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign)
    {
        auto hdc = g_hdc;
        auto flags = convertToTextFlags(horizAlign, vertAlign);

        RECT _rc = {
            .left = rc.left,
            .top = rc.top,
            .right = rc.right,
            .bottom = rc.bottom,
        };

        DrawTextA(hdc, (char*)text, -1, &_rc, flags | DT_CALCRECT);

        rc.left = (coord_t)_rc.left;
        rc.top = (coord_t)_rc.top;
        rc.right = (coord_t)_rc.right;
        rc.bottom = (coord_t)_rc.bottom;
    };

    context.text = [](const rect_t& rc, const char* text, color_t color, color_t bgColor, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign)
    {
        auto hdc = g_hdc;
        auto flags = convertToTextFlags(horizAlign, vertAlign);

        SetTextColor(hdc, color);
        SetBkMode(hdc, OPAQUE);
        SetBkColor(hdc, (COLORREF)bgColor);

        RECT _rc = {
           .left = rc.left,
           .top = rc.top,
           .right = rc.right,
           .bottom = rc.bottom,
        };

        DrawTextA(hdc, (char*)text, -1, &_rc, flags);
    };
}
