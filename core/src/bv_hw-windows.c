/*
 * File     bv_hw-windows.c
 * Date     28.12.2021
 */

#ifdef _WINDOWS


#include "../inc/bv_hwdriver.h"
#include <Windows.h>
#include "../../config_win32.h"


extern HDC g_hdc;


//#define USING_STOCK_FONT        OEM_FIXED_FONT
//#define USING_STOCK_FONT        ANSI_FIXED_FONT
//#define USING_STOCK_FONT        ANSI_VAR_FONT
//#define USING_STOCK_FONT        SYSTEM_FONT
//#define USING_STOCK_FONT        DEVICE_DEFAULT_FONT
//#define USING_STOCK_FONT        DEFAULT_PALETTE
#define USING_STOCK_FONT        SYSTEM_FIXED_FONT
//#define USING_STOCK_FONT        DEFAULT_GUI_FONT


VOID ConvertCoordinatePoint(
    point_t* ppt)
{
    ppt->x += CTL_BUTTON_ZONE_SIZE;
    ppt->y += CTL_BUTTON_ZONE_SIZE;
}

VOID ConvertCoordinateRect(
    rect_t* prc)
{
    RECT_Offset(prc, CTL_BUTTON_ZONE_SIZE, CTL_BUTTON_ZONE_SIZE);
}

UINT BVG_ConvertAlignToTextFlags(
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
    point_t pt = *ppt;
    ConvertCoordinatePoint(&pt);
    SetPixel(g_hdc, pt.x, pt.y, color);
}

void BVG_DrawLine(
    const point_t* ppt0,
    const point_t* ppt1,
    uint8_t thickness,
    color_t color)
{
    point_t pt0 = *ppt0;
    point_t pt1 = *ppt1;
    ConvertCoordinatePoint(&pt0);
    ConvertCoordinatePoint(&pt1);

    HPEN hPen = CreatePen(PS_SOLID, thickness, color);
    HPEN hPenOrigin = SelectObject(g_hdc, hPen);
    MoveToEx(g_hdc, pt0.x, pt0.y, 0);
    LineTo(g_hdc, pt1.x, pt1.y);
    SelectObject(g_hdc, hPenOrigin);
    DeleteObject(hPen);
}

void BVG_DrawRect(
    const rect_t* prc,
    uint8_t thickness,
    color_t color,
    color_t bgColor)
{
    rect_t rc = *prc;
    ConvertCoordinateRect(&rc);
    HPEN pen = CreatePen(PS_SOLID, thickness, color);
    HPEN penOrigin = SelectObject(g_hdc, pen);
    HBRUSH brushOrigin = SelectObject(g_hdc, GetStockObject(NULL_BRUSH));
    Rectangle(g_hdc, (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);
    SelectObject(g_hdc, brushOrigin);
    SelectObject(g_hdc, penOrigin);
    DeleteObject(pen);
}

void BVG_DrawFill(
    const rect_t* prc,
    color_t color)
{
    rect_t rc = *prc;
    ConvertCoordinateRect(&rc);
    HPEN penOrigin = SelectObject(g_hdc, GetStockObject(DC_PEN));
    HBRUSH brushOrigin = SelectObject(g_hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(g_hdc, color);
    SetDCPenColor(g_hdc, color);
    Rectangle(g_hdc, (int)rc.left, (int)rc.top, (int)rc.right, (int)rc.bottom);
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
    DWORD flags = BVG_ConvertAlignToTextFlags(hAlign, vAlign);

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
    uint16_t nLen,
    color_t foreColor,
    color_t backColor,
    horizontal_aligment_t hAlign,
    vertical_aligment_t vAlign)
{
    rect_t rc = *prc;
    ConvertCoordinateRect(&rc);

    DWORD flags = BVG_ConvertAlignToTextFlags(hAlign, vAlign);

    SetTextColor(g_hdc, foreColor);
    SetBkMode(g_hdc, OPAQUE);
    SetBkColor(g_hdc, (COLORREF)backColor);

    RECT rcText = {
       .left = rc.left,
       .top = rc.top,
       .right = rc.right,
       .bottom = rc.bottom,
    };

    HFONT hFont = GetStockObject(USING_STOCK_FONT);
    HFONT hFontOrigin = SelectObject(g_hdc, hFont);

    DrawTextA(g_hdc, (char*)szText, nLen, &rcText, flags);

    SelectObject(g_hdc, hFontOrigin);
}

void BVG_Polygon(
    point_t* ppts,
    uint16_t nPointsCount,
    uint8_t thickness,
    color_t color,
    color_t bgColor)
{
    HPEN hPen = CreatePen(PS_SOLID, thickness, color);
    HBRUSH hBrush = CreateSolidBrush(bgColor);
    HPEN hPenOrigin = SelectObject(g_hdc, hPen);
    HBRUSH hBrushOrigin = SelectObject(g_hdc, hBrush);

    POINT* pts = (POINT*)malloc(sizeof(POINT) * nPointsCount);
    if (pts)
    {
        for (int i = 0; i < (int)nPointsCount; i++)
        {
            POINT* pptDest = &pts[i];
            point_t* pptSrc = &ppts[i];

            point_t pt = ppts[i];
            ConvertCoordinatePoint(&pt);

            pptDest->x = (LONG)pt.x;
            pptDest->y = (LONG)pt.y;
        }

        Polygon(g_hdc, pts, nPointsCount);

        free(pts);
    }

    SelectObject(g_hdc, hBrushOrigin);
    SelectObject(g_hdc, hPenOrigin);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

#endif


/* END OF FILE */
