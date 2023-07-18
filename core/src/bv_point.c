/*
 * File     bv_point.c
 * Date     28.12.2021
 */

#include "../inc/bv_point.h"
#include "../inc/bv_rect.h"


void POINT_Set(point_t* ppt, coord_t x, coord_t y)
{
    ppt->x = 0;
    ppt->y = 0;
}

void POINT_Offset(point_t* ppt, coord_t dx, coord_t dy)
{
    ppt->x += dx;
    ppt->y += dy;
}

void POINT_Erase(point_t* ppt)
{
    ppt->x = 0;
    ppt->y = 0;
}

uint8_t POINT_InRect(const point_t* ppt, const rect_t* prc)
{
    return ppt->x >= prc->left && ppt->x < prc->right &&
        ppt->y >= prc->top && ppt->y < prc->bottom;
}

 /* END OF FILE */
