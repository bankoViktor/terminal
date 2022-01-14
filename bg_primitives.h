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

extern void BVP_DrawInput(
    const uint8_t* szValue,
    uint16_t wLengthMax,
    uint16_t wCursorPos);

extern void BVP_DrawMessage(
    const uint8_t* szMessage,
    color_t foreColor,
    color_t backColor);

extern inline void BVP_DrawErrorMessage(
    const uint8_t* szMessage);


#endif // !__BV_PRIMITIVES_H

 /* END OF FILE */
