#pragma once

#include "types.h"


using render_context_t = struct render_context_t
{
    using moveTo_f = void(*)(int16_t x, int16_t y);
    using lineTo_f = void(*)(int16_t x, int16_t y, color_t color);
    using rect_f = void(*)(const rect_t& rc, color_t color);
    using calcText_f = void(*)(rect_t& rc, const char* text, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign);
    using text_f = void(*)(const rect_t& rc, const char* text, color_t color, color_t bgColor, horizontal_aligment_t horizAlign, vertical_aligment_t vertAlign);

    rect_t rc;

    moveTo_f moveTo;
    lineTo_f lineTo;
    rect_f rect;
    calcText_f calcText;
    text_f text;
};

void initRenderContext(render_context_t& context, const rect_t& rc);
