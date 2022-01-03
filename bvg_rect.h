/*
 * File     bvg_rect.h
 * Date     28.12.2021
 */

#ifndef __BVG_RECT_H
#define __BVG_RECT_H


#include "bvg_types.h"


typedef struct point_t point_t;


typedef struct rect_t
{
    coord_t left;
    coord_t top;
    coord_t right;
    coord_t bottom;
} rect_t;


extern void RECT_Erase(
    rect_t* prc);

extern void RECT_Set(
    rect_t* prc,
    coord_t left,
    coord_t top,
    coord_t right,
    coord_t bottom);

extern void RECT_SetWithSize(
    rect_t* prc,
    coord_t left,
    coord_t top,
    coord_t width,
    coord_t height);

extern void RECT_Offset(
    rect_t* prc,
    coord_t dx,
    coord_t dy);

extern inline void RECT_OffsetToPoint(
    rect_t* prc,
    const point_t* ppt);

extern void RECT_Inflate(
    rect_t* prc,
    coord_t dx,
    coord_t dy);

extern inline coord_t RECT_GetWidth(
    const rect_t* prc);

extern inline coord_t RECT_GetHeight(
    const rect_t* prc);

extern uint8_t RECT_IsEmpty(
    const rect_t* prc);

extern point_t RECT_GetLT(
    const rect_t* prc);

extern point_t RECT_GetRB(
    const rect_t* prc);


#endif // !__BVG_RECT_H

/* END OF FILE */
