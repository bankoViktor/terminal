/*
 * File     bvg_hwdriver_win32.c
 * Date     28.12.2021
 */

#include "bvg_hwdriver.h"
#include <Windows.h>

extern HDC g_hdc;


inline void BVG_DrawPixel(const point_t* ppt, color_t color)
{
    SetPixel(g_hdc, ppt->x, ppt->y, color);
}

void BVG_DrawLine(const point_t* ppt0, const point_t* ppt1, uint8_t thickness, color_t color)
{
    HPEN hPen = CreatePen(PS_SOLID, thickness, color);
    HPEN hPenOrigin = SelectObject(g_hdc, hPen);
    MoveToEx(g_hdc, ppt0->x, ppt0->y, 0);
    LineTo(g_hdc, ppt1->x, ppt1->y);
    SelectObject(g_hdc, hPenOrigin);
    DeleteObject(hPen);
}

void BVG_DrawRect(const rect_t* prc, uint8_t thickness, color_t color, color_t bgColor)
{

}

void BVG_DrawFill(const rect_t* prc, color_t color)
{
}

void BVG_DrawArc(const point_t* ppt, uint16_t radius, uint8_t thickness, color_t color)
{
}


/* END OF FILE */
