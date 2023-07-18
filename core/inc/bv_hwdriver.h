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

// Вычисляет размеры прямоугольника (переопределяя переданный объект).
extern void BVG_CalcText(
    rect_t* prc,                    // Указатель на объект прямоугольника.
    const char* szText,             // Текст.
    horizontal_aligment_t hAlign,   // Горизонтальное выравнивание.
    vertical_aligment_t vAlign);    // Вертикальное выравнивание.

// Рисует текст в заданном прямоугольнике.
extern void BVG_DrawText(
    const rect_t* prc,              // Указатель на объект прямоугольника.
    const char* szText,             // Текст.
    uint16_t nLen,                  // Количество символов текста.
    color_t foreColor,              // Цвет текста.
    color_t backColor,              // Цвет заднего фона.
    horizontal_aligment_t hAlign,   // Горизонтальное выравнивание.
    vertical_aligment_t vAlign);    // Вертикальное выравнивание.

// Рисует полигон по заданным точкам.
extern void BVG_Polygon(
    point_t* ppts,                  // Указатель на массив точек.
    uint16_t nPointsCount,          // Количество точек в массиве.
    uint8_t thickness,              // Толщина линии обводки.
    color_t color,                  // Цвет линии обводки.
    color_t bgColor);               // Цвет заливки.

#endif // __BV_HWDRIVER_H

/* END OF FILE */
