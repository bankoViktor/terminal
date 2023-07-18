/*
 * File     bv_primitives.h
 * Date     04.01.2022
 */

#ifndef __BV_PRIMITIVES_H
#define __BV_PRIMITIVES_H


#include "bv_rect.h"
#include "bv_frame.h"


// Ориентация треуголиника.
typedef enum triangle_orientation_t
{
    TO_LEFT,
    TO_UP,
    TO_RIGHT,
    TO_DOWN,
} triangle_orientation_t;


void BVP_DrawDirectionSymbol(
    const rect_t* pRc,
    triangle_orientation_t eOrientation,
    color_t color);

void BVP_DrawInput(
    const uint8_t* pszText,
    uint16_t nLengthMax,
    uint16_t nCursorPos);

void BVP_DrawMessage(
    const uint8_t* pszMessage,
    color_t foreColor,
    color_t backColor);

void BVP_DrawErrorMessage(
    const uint8_t* pszMessage);

void BVP_DrawClientRect();

void BVP_DrawButtonGrid();

void BVP_DrawMarker(
    const point_t* pPt,
    color_t color);

void BVP_DrawButtonText(
    uint8_t nButtonIndex,
    double dOffset,
    coord_t nOffsetFromSafe,
    const uint8_t* pszText,
    color_t foreColor,
    color_t backColor);

void BVP_DrawButtonMarker(
    uint8_t nButtonIndex,
    const point_t* pPt,
    button_type_t type);

void BVP_DrawDSMS(
    const rect_t* pRc);


#endif // !__BV_PRIMITIVES_H

/* END OF FILE */
