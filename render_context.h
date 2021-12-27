#pragma once

#include "types.h"


using render_context_t = struct render_context_t
{
    rect_t rc;

    void moveTo(int16_t x, int16_t y) const;
    void lineTo(int16_t x, int16_t y, color_t color, uint16_t thickness = 1) const;
    void rect(const rect_t& rc, color_t color, uint16_t thickness = 1) const;
    void fill(const rect_t& rc, color_t color) const;
    void calcText(rect_t& rc, const char* text, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign) const;
    void text(const rect_t& rc, const char* text, color_t color, color_t bgColor, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign) const;
    void arc(const rect_t& rc, color_t color) const;
    void angleArc(const point_t& pt, coord_t radius, uint16_t startAngle, uint16_t sweepAngle, color_t color, uint8_t thickness = 1) const;
};
