#pragma once

#include "framework.h"


struct render_context_t
{
    using moveTo_t = void(*)(int x, int y);
    using lineTo_t = void(*)(int x, int y, int color);
    using rect_t = void(*)(int x, int y, int cx, int cy, int color);
    using text_t = void(*)(RECT* pRC, const char* text, int color, int bgColor, int flags);
    using calcText_t = void(*)(RECT* pRC, const char* text, int flags);

    int width;
    int height;

    moveTo_t moveTo;
    lineTo_t lineTo;
    rect_t rect;
    text_t text;
    calcText_t calcText;
};

void initRenderContext(render_context_t& rc);
