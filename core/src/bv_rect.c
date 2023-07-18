/*
 * File     bv_rect.c
 * Date     28.12.2021
 */

#include "../inc/bv_rect.h"
#include "../inc/bv_point.h"


void RECT_Erase(rect_t* prc)
{
    prc->left = 0;
    prc->top = 0;
    prc->right = 0;
    prc->bottom = 0;
}

void RECT_Set(rect_t* prc, coord_t left, coord_t top, coord_t right, coord_t bottom)
{
    prc->left = left;
    prc->top = top;
    prc->right = right;
    prc->bottom = bottom;
}

void RECT_SetWithSize(rect_t* prc, coord_t left, coord_t top, coord_t width, coord_t height)
{
    prc->left = left;
    prc->top = top;
    prc->right = left + width;
    prc->bottom = top + height;
}

void RECT_Offset(rect_t* prc, coord_t dx, coord_t dy)
{
    prc->left += dx;
    prc->top += dy;
    prc->right += dx;
    prc->bottom += dy;
}

void RECT_OffsetToPoint(rect_t* prc, const point_t* ppt)
{
    prc->left += ppt->x;
    prc->top += ppt->y;
    prc->right += ppt->x;
    prc->bottom += ppt->y;
}

void RECT_Inflate(rect_t* prc, coord_t dx, coord_t dy)
{
    prc->left -= dx;
    prc->top -= dy;
    prc->right += dx;
    prc->bottom += dy;
}

coord_t RECT_GetWidth(const rect_t* prc)
{
    return prc->right - prc->left;
}

coord_t RECT_GetHeight(const rect_t* prc)
{
    return prc->bottom - prc->top;
}

uint8_t RECT_IsEmpty(const rect_t* prc)
{
    return RECT_GetWidth(prc) == 0 && RECT_GetHeight(prc) == 0;
}

point_t RECT_GetLT(const rect_t* prc)
{
    point_t pt = { prc->left, prc->top };
    return pt;
}

point_t RECT_GetRB(const rect_t* prc)
{
    point_t pt = { prc->right, prc->bottom };
    return pt;
}

/* END OF FILE */
