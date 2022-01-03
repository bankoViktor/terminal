/*
 * File     bvg_hwdriver.h
 * Date     28.12.2021
 */

#ifndef __BVG_HWDRIVER_H
#define __BVG_HWDRIVER_H

#include "bvg_rect.h"
#include "bvg_point.h"


// ������ �������.
extern void BVG_DrawPixel(
    const point_t*  ppt,            // ���������� �������.
    color_t         color);         // ���� �������.

// ������ ����� �� ���� �����������.
extern void BVG_DrawLine(
    const point_t*  ppt0,           // ���������� ��������� �����.
    const point_t*  ppt1,           // ���������� ��������� �����.
    uint8_t         thickness,      // ������� �����.
    color_t         color);         // ���� �����.

// ������ ������ �������������.
extern void BVG_DrawRect(
    const rect_t*   prc,            // �������� ��������������.
    uint8_t         thickness,      // ������� �����.
    color_t         color,          // ���� �����.
    color_t         bgColor);       // ���� ���������� ��������������.

// ������ ������� �������������.
extern void BVG_DrawFill(
    const rect_t*   prc,            // �������� ��������������.
    color_t         color);         // ���� ���������� ��������������.

// ������ ���� ����������.
extern void BVG_DrawArc(
    const point_t*  ppt0,           // ����� ����
    uint16_t        radius,         // ������ ����
    uint8_t         thickness,      // ������� �����
    color_t         color);         // ���� �����

extern void BVG_CalcText(
    rect_t* prc,
    const char* szText,
    horizontal_aligment_t hAlign,
    vertical_aligment_t vAlign);

extern void BVG_DrawText(
    const rect_t* prc,
    const char* szText,
    color_t color,
    color_t bgColor,
    horizontal_aligment_t hAlign,
    vertical_aligment_t vAlign);

#endif // __BVG_HWDRIVER_H

/* END OF FILE */
