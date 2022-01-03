/*
 * File     bv_hwdriver_win32.c
 * Date     28.12.2021
 */

#include "bv_hwdriver.h"
#include <Windows.h>

extern HDC g_hdc;


#define USING_STOCK_FONT    DEVICE_DEFAULT_FONT


UINT convertToTextFlags(
    horizontal_aligment_t horizAlign,
    vertical_aligment_t vertAlign)
{
    UINT result = DT_NOPREFIX | DT_HIDEPREFIX;

    // Horizontal 
    result &= ~(DT_LEFT | DT_CENTER | DT_RIGHT);
    switch (horizAlign)
    {
    case H_ALIGN_LEFT:
        result |= DT_LEFT;
        break;
    case H_ALIGN_CENTER:
        result |= DT_CENTER;
        break;
    case H_ALIGN_RIGHT:
        result |= DT_RIGHT;
        break;
    }

    // Vertical 
    result &= ~(DT_TOP | DT_VCENTER | DT_BOTTOM);
    switch (vertAlign)
    {
    case V_ALIGN_TOP:
        result |= DT_TOP;
        break;
    case V_ALIGN_MIDDLE:
        result |= DT_VCENTER;
        break;
    case V_ALIGN_BOTTOM:
        result |= DT_BOTTOM;
        break;
    }

    return result;
}


inline void BVG_DrawPixel(
    const point_t* ppt,
    color_t color)
{
    SetPixel(g_hdc, ppt->x, ppt->y, color);
}

void BVG_DrawLine(
    const point_t* ppt0,
    const point_t* ppt1,
    uint8_t thickness,
    color_t color)
{
    HPEN hPen = CreatePen(PS_SOLID, thickness, color);
    HPEN hPenOrigin = SelectObject(g_hdc, hPen);
    MoveToEx(g_hdc, ppt0->x, ppt0->y, 0);
    LineTo(g_hdc, ppt1->x, ppt1->y);
    SelectObject(g_hdc, hPenOrigin);
    DeleteObject(hPen);
}

void BVG_DrawRect(
    const rect_t* prc,
    uint8_t thickness,
    color_t color,
    color_t bgColor)
{
    HPEN pen = CreatePen(PS_SOLID, thickness, color);
    HPEN penOrigin = SelectObject(g_hdc, pen);
    HBRUSH brushOrigin = SelectObject(g_hdc, GetStockObject(NULL_BRUSH));
    Rectangle(g_hdc, (int)prc->left, (int)prc->top, (int)prc->right, (int)prc->bottom);
    SelectObject(g_hdc, brushOrigin);
    SelectObject(g_hdc, penOrigin);
    DeleteObject(pen);
}

void BVG_DrawFill(
    const rect_t* prc,
    color_t color)
{
    HPEN penOrigin = SelectObject(g_hdc, GetStockObject(DC_PEN));
    HBRUSH brushOrigin = SelectObject(g_hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(g_hdc, color);
    SetDCPenColor(g_hdc, color);
    Rectangle(g_hdc, (int)prc->left, (int)prc->top, (int)prc->right, (int)prc->bottom);
    SelectObject(g_hdc, brushOrigin);
    SelectObject(g_hdc, penOrigin);
}

void BVG_DrawArc(
    const point_t* ppt,
    uint16_t radius,
    uint8_t thickness,
    color_t color)
{
    //HPEN pen = CreatePen(PS_SOLID, 3, color);
    //HPEN penOrigin = SelectObject(hdc, pen);
    //Arc(hdc, (int)prc->left, rc.top, rc.bottom, rc.right, 100, 200, 0, 100);
    //SelectObject(hdc, penOrigin);
    //DeleteObject(pen);
}

//void angleArc(const point_t& pt, uint16_t radius, uint8_t startAngle, uint8_t sweepAngle, color_t color, uint8_t thickness)
//{
//    auto hdc = g_hdc;
//    auto pen = CreatePen(PS_SOLID, thickness, color);
//    auto penOrigin = SelectObject(hdc, pen);
//    MoveToEx(hdc, pt.x - radius, pt.y, NULL);
//    AngleArc(hdc, pt.x, pt.y, radius, startAngle, sweepAngle);
//    SelectObject(hdc, penOrigin);
//    DeleteObject(pen);
//}

void BVG_CalcText(
    rect_t* prc,
    const char* szText,
    horizontal_aligment_t hAlign,
    vertical_aligment_t vAlign)
{
    DWORD flags = convertToTextFlags(hAlign, vAlign);

    RECT rc = {
        .left = prc->left,
        .top = prc->top,
        .right = prc->right,
        .bottom = prc->bottom,
    };

    HFONT hFont = GetStockObject(USING_STOCK_FONT);
    HFONT hFontOrigin = SelectObject(g_hdc, hFont);

    DrawTextA(g_hdc, (char*)szText, -1, &rc, flags | DT_CALCRECT);

    SelectObject(g_hdc, hFontOrigin);

    prc->left = (coord_t)rc.left;
    prc->top = (coord_t)rc.top;
    prc->right = (coord_t)rc.right;
    prc->bottom = (coord_t)rc.bottom;
}

void BVG_DrawText(
    const rect_t* prc,
    const char* szText,
    color_t color,
    color_t bgColor,
    horizontal_aligment_t hAlign,
    vertical_aligment_t vAlign)
{
    DWORD flags = convertToTextFlags(hAlign, vAlign);

    SetTextColor(g_hdc, color);
    SetBkMode(g_hdc, OPAQUE);
    SetBkColor(g_hdc, (COLORREF)bgColor);

    RECT rc = {
       .left = prc->left,
       .top = prc->top,
       .right = prc->right,
       .bottom = prc->bottom,
    };

    HFONT hFont = GetStockObject(USING_STOCK_FONT);
    HFONT hFontOrigin = SelectObject(g_hdc, hFont);

    DrawTextA(g_hdc, (char*)szText, -1, &rc, flags);

    SelectObject(g_hdc, hFontOrigin);
}


/* END OF FILE */
