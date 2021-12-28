/*
 * File     bvg_point.h
 * Date     28.12.2021
 */

#ifndef __BVG_POINT_H
#define __BVG_POINT_H

#include "bvg_types.h"

typedef struct rect_t rect_t;


typedef struct point_t
{
    coord_t x;
    coord_t y;
} point_t;


void POINT_Set(
    point_t* ppt,
    coord_t x,
    coord_t y);

void POINT_Offset(
    point_t* ppt,
    coord_t dx,
    coord_t dy);

void POINT_Erase(
    point_t* ppt);

uint8_t POINT_InRect(
    const point_t* ppt,
    const rect_t* prc);

#endif // !__BVG_POINT_H

/* END OF FILE */
