/*
 * File     bv_primitives.h
 * Date     04.01.2022
 */

#ifndef __BV_PRIMITIVES_H
#define __BV_PRIMITIVES_H

#include "bv_rect.h"


typedef enum triangle_orientation_t
{
    TO_LEFT,
    TO_UP,
    TO_RIGHT,
    TO_DOWN,
} triangle_orientation_t;


extern void BVP_DrawDirectionSymbol(
    const rect_t* prc,
    triangle_orientation_t orientation,
    color_t color);


#endif // !__BV_PRIMITIVES_H

 /* END OF FILE */
