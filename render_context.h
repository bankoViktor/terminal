#pragma once

#include "types.h"


using render_context_t = struct render_context_t
{
    rect_t rc;

    void calcButtonPos(uint8_t index, point_t& pt, int16_t delta = 0) const;
    void getClientRect(rect_t& rc) const;

    void moveTo(int16_t x, int16_t y) const;
    void lineTo(int16_t x, int16_t y, color_t color) const;
    void rect(const rect_t& rc, color_t color) const;
    void calcText(rect_t& rc, const char* text, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign) const;
    void text(const rect_t& rc, const char* text, color_t color, color_t bgColor, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign) const;
};
