/*
 * File     bv_hwdriver.h
 * Date     28.12.2021
 */

#ifndef __BV_HWDRIVER_H
#define __BV_HWDRIVER_H

#include "bv_rect.h"
#include "bv_point.h"


#define TRANSPARENT_COLOR           ((uint32_t)(0xff000000))

// Рисует пиксель.
extern void BVG_DrawPixel(
    const point_t*  ppt,            // Координаты пикселя.
    color_t         color);         // Цвет пикселя.

// Рисует линию по двум координатам.
extern void BVG_DrawLine(
    const point_t*  ppt0,           // Координаты начальной точки.
    const point_t*  ppt1,           // Координаты конечного точки.
    uint8_t         thickness,      // Толщина линии.
    color_t         color);         // Цвет линии.

// Рисует пустой прямоугольник.
extern void BVG_DrawRect(
    const rect_t*   prc,            // Габариты прямоуголтника.
    uint8_t         thickness,      // Толщина линий.
    color_t         color,          // Цвет линий.
    color_t         bgColor);       // Цвет заполнения прямоугольника.

// Рисует залитый прямоугольник.
extern void BVG_DrawFill(
    const rect_t*   prc,            // Габариты прямоуголтника.
    color_t         color);         // Цвет заполнения прямоугольника.

// Рисует дугу окружности.
extern void BVG_DrawArc(
    const point_t*  ppt0,           // Центр дуги
    uint16_t        radius,         // Радиус дуги
    uint8_t         thickness,      // Толщина линии
    color_t         color);         // Цвет линии

extern void BVG_CalcText(
    rect_t* prc,
    const char* szText,
    horizontal_aligment_t hAlign,
    vertical_aligment_t vAlign);

extern void BVG_DrawText(
    const rect_t* prc,
    const char* szText,
    uint16_t nLen,
    color_t foreColor,
    color_t backColor,
    horizontal_aligment_t hAlign,
    vertical_aligment_t vAlign);

extern void BVG_Polygon(
    const point_t* ppts,
    uint16_t nPointsCount,
    uint8_t thickness,
    color_t color,
    color_t bgColor);

#endif // __BV_HWDRIVER_H

/* END OF FILE */
