/*
 * File     bv_primitives.c
 * Date     04.01.2022
 */

#include "bg_primitives.h"
#include "bv_hwdriver.h"


void BVP_DrawDirectionSymbol(
    const rect_t* prc,
    triangle_orientation_t orientation,
    color_t color)
{
    point_t pt[3] = { 0 };

    coord_t hx = RECT_GetWidth(prc) / 2;
    coord_t hy = RECT_GetHeight(prc) / 2;

    switch (orientation)
    {
    case TO_LEFT:
        pt[0].x = prc->left;
        pt[0].y = prc->top + hy;
        pt[1].x = prc->right;
        pt[1].y = prc->top;
        pt[2].x = prc->right;
        pt[2].y = prc->bottom;
        break;
    case TO_UP:
        pt[0].x = prc->left + hx;
        pt[0].y = prc->top;
        pt[1].x = prc->right;
        pt[1].y = prc->bottom;
        pt[2].x = prc->left;
        pt[2].y = prc->bottom;
        break;
    case TO_RIGHT:
        pt[0].x = prc->right;
        pt[0].y = prc->top + hy;
        pt[1].x = prc->left;
        pt[1].y = prc->bottom;
        pt[2].x = prc->left;
        pt[2].y = prc->top;
        break;
    case TO_DOWN:
        pt[0].x = prc->left + hx;
        pt[0].y = prc->bottom;
        pt[1].x = prc->left;
        pt[1].y = prc->top;
        pt[2].x = prc->right;
        pt[2].y = prc->top;
        break;
    }

    BVG_Polygon(pt, 3, 1, color, color);
}


 /* END OF FILE */
