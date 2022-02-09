/*
 * File     bv_primitives.h
 * Date     04.01.2022
 */

#ifndef __BV_PRIMITIVES_H
#define __BV_PRIMITIVES_H

#include "bv_rect.h"
#include "bv_frame.h"


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

extern void BVP_DrawClientRect();

extern void BVP_DrawButtonGrid();

extern void BVP_DrawMarker(
    const point_t* ppt,
    color_t color);

extern void BVP_DrawButtonText(
    uint8_t nIndex,
    coord_t nOffset,
    const uint8_t* szText,
    color_t foreColor,
    color_t backColor);

extern void BVP_DrawButtonMarker(
    uint8_t nIndex,
    const point_t* ppt,
    button_type_t type);

extern void BVP_DrawDSMS(
    const rect_t* prc);


#endif // !__BV_PRIMITIVES_H

/* END OF FILE */
