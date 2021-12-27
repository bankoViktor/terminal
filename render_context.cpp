
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

void render_context_t::moveTo(int16_t x, int16_t y) const
{
    auto hdc = g_hdc;
    MoveToEx(hdc, (int)x, (int)y, NULL);
}

void render_context_t::lineTo(int16_t x, int16_t y, color_t color, uint16_t thickness) const
{
    auto hdc = g_hdc;
    auto pen = CreatePen(PS_SOLID, thickness, color);
    auto penOrigin = SelectObject(hdc, pen);
    SetDCPenColor(hdc, (COLORREF)color);
    LineTo(hdc, (int)x, (int)y);
    SelectObject(hdc, penOrigin);
    DeleteObject(pen);
}

void render_context_t::rect(const rect_t& rc, color_t color, uint16_t thickness) const
{
    auto hdc = g_hdc;
    auto pen = CreatePen(PS_SOLID, thickness, color);
    auto penOrigin = SelectObject(hdc, pen);
    auto brushOrigin = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);
    SelectObject(hdc, brushOrigin);
    SelectObject(hdc, penOrigin);
    DeleteObject(pen);
}

void render_context_t::fill(const rect_t& rc, color_t color) const
{
    auto hdc = g_hdc;
    auto penOrigin = SelectObject(hdc, GetStockObject(DC_PEN));
    auto brushOrigin = SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, color);
    SetDCPenColor(hdc, color);
    Rectangle(hdc, (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);
    SelectObject(hdc, brushOrigin);
    SelectObject(hdc, penOrigin);
}

void render_context_t::calcText(rect_t& rc, const char* text, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign) const
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
}

void render_context_t::text(const rect_t& rc, const char* text, color_t color, color_t bgColor, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign) const
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
}

void render_context_t::arc(const rect_t& rc, color_t color) const
{
    auto hdc = g_hdc;
    auto pen = CreatePen(PS_SOLID, 3, color);
    auto penOrigin = SelectObject(hdc, pen);
    Arc(hdc, rc.left, rc.top, rc.bottom, rc.right, 100, 200, 0, 100);
    SelectObject(hdc, penOrigin);
    DeleteObject(pen);
}

void render_context_t::angleArc(const point_t& pt, coord_t radius, uint16_t startAngle, uint16_t sweepAngle, color_t color, uint8_t thickness) const
{
    auto hdc = g_hdc;
    auto pen = CreatePen(PS_SOLID, thickness, color);
    auto penOrigin = SelectObject(hdc, pen);
    MoveToEx(hdc, pt.x - radius, pt.y, NULL);
    AngleArc(hdc, pt.x, pt.y, radius, startAngle, sweepAngle);
    SelectObject(hdc, penOrigin);
    DeleteObject(pen);
}
