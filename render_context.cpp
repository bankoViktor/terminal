
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

void render_context_t::calcButtonPos(uint8_t index, point_t& pt, int16_t delta) const
{
    rect_t rc = this->rc;

    auto stepX = (double)rc.width() / (BUTTON_COUNT_X + 1);
    auto stepY = (double)rc.height() / (BUTTON_COUNT_Y + 1);

    if (index < BUTTONS_TOP)
    {
        // Top
        pt.x = rc.left + (coord_t)(stepX * (index + 1));
        pt.y = rc.top + delta;
    }
    else if (index < BUTTONS_RIGHT)
    {
        // Right
        pt.x = rc.right - 1 - delta;
        pt.y = rc.top + (coord_t)(stepY * (index - BUTTONS_TOP + 1));
    }
    else if (index < BUTTONS_BOTTOM)
    {
        // Bottom
        pt.x = rc.left + (coord_t)(stepX * (BUTTON_COUNT_X - index + BUTTONS_RIGHT));
        pt.y = rc.bottom - 1 - delta;
    }
    else if (index < BUTTONS_LEFT)
    {
        // Left
        pt.x = rc.left + delta;
        pt.y = rc.top + (coord_t)(stepY * (BUTTON_COUNT_Y - index + BUTTONS_BOTTOM));
    }
}

void render_context_t::getClientRect(rect_t& rc) const
{
    rc = this->rc;
    rc.inflate(-SAFE_OFFSET);
}

void render_context_t::moveTo(int16_t x, int16_t y) const
{
    auto hdc = g_hdc;
    MoveToEx(hdc, (int)x, (int)y, NULL);
}

void render_context_t::lineTo(int16_t x, int16_t y, color_t color) const
{
    auto hdc = g_hdc;
    auto pen = GetStockObject(DC_PEN);
    auto penOrigin = SelectObject(hdc, pen);
    SetDCPenColor(hdc, (COLORREF)color);
    LineTo(hdc, (int)x, (int)y);
    SelectObject(hdc, penOrigin);
}

void render_context_t::rect(const rect_t& rc, color_t color) const
{
    auto hdc = g_hdc;
    auto hPenOrigin = SelectObject(hdc, GetStockObject(DC_PEN));
    auto hBrushOrigin = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    SetDCPenColor(hdc, color);
    Rectangle(hdc, (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);

    SelectObject(hdc, hPenOrigin);
    SelectObject(hdc, hBrushOrigin);
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
