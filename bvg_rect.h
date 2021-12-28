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


void RECT_Erase(
    rect_t* prc);

void RECT_Set(
    rect_t* prc,
    coord_t left,
    coord_t top,
    coord_t right,
    coord_t bottom);

void RECT_SetSize(
    rect_t* prc,
    coord_t left,
    coord_t top,
    coord_t width,
    coord_t height);

void RECT_Offset(
    rect_t* prc,
    coord_t dx,
    coord_t dy);

void RECT_Inflate(
    rect_t* prc,
    coord_t dx,
    coord_t dy);

coord_t RECT_GetWidth(
    const rect_t* prc);

coord_t RECT_GetHeight(
    const rect_t* prc);

uint8_t RECT_IsEmpty(
    const rect_t* prc);

point_t RECT_GetLT(
    const rect_t* prc);

point_t RECT_GetRB(
    const rect_t* prc);


#endif // !__BVG_RECT_H

/* END OF FILE */
